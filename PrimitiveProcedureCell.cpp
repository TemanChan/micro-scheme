#include "PrimitiveProcedureCell.hpp"
#include "IntCell.hpp"
#include "DoubleCell.hpp"
#include "SymbolCell.hpp"
#include "ConsCell.hpp"
#include <functional>
#include <set>
#include <iterator>
#include <cstdlib> // exit
#include <fstream>
using namespace std;

list<map<string, CellPtr> > symbol_table(1, PrimitiveProcedureCell::create_map());


PrimitiveProcedureCell::PrimitiveProcedureCell(CellPtr (*func)(CellPtr const))
	:ProcedureCell(smart_nil, smart_nil), func_m(func)
{

}

PrimitiveProcedureCell::~PrimitiveProcedureCell()
{

}

CellPtr PrimitiveProcedureCell::apply(CellPtr const args)
{
	return func_m(args);
}

void PrimitiveProcedureCell::print(ostream& os) const
{
	os << "#<primitive>";
}

CellPtr PrimitiveProcedureCell::add(CellPtr const args)
{
	if(args->is_nil())
		return make_shared<IntCell>(0);
	else
		return arithmetic_operation(args, plus<int>(), plus<double>(), "+");
}

CellPtr PrimitiveProcedureCell::subtract(CellPtr const args)
{
	if(args->is_nil())
		throw runtime_error("- operator requires at least one operand");
	else if(args->get_cdr()->is_nil()){
		CellPtr operand = args->get_car()->eval();
		if(operand->is_int())
			return make_shared<IntCell>(0 - operand->get_int());
		else if(operand->is_double())
			return make_shared<DoubleCell>(0 - operand->get_double());
		else
			throw runtime_error("operant for - is neither an int nor a double");
	}
	else
		return arithmetic_operation(args, minus<int>(), minus<double>(), "-");
}

CellPtr PrimitiveProcedureCell::multiply(CellPtr const args)
{
	if(args->is_nil())
		return make_shared<IntCell>(1);
	else
		return arithmetic_operation(args, multiplies<int>(), multiplies<double>(), "*");
}

CellPtr PrimitiveProcedureCell::divide(CellPtr const args)
{
	if(args->is_nil())
		throw runtime_error("/ operator requires at least one operand");
	else if(args->get_cdr()->is_nil()){
		CellPtr operand = args->get_car()->eval();
		if(operand->is_int())
			return make_shared<IntCell>(1 / operand->get_int());
		else if(operand->is_double())
			return make_shared<DoubleCell>(1.0 / operand->get_double());
		else
			throw runtime_error("operant for / is neither an int nor a double");
	}
	else
		return arithmetic_operation(args, divides<int>(), divides<double>(), "/");
}

CellPtr PrimitiveProcedureCell::less_than(CellPtr const args)
{
	if(args->is_nil())
		return make_shared<IntCell>(1);
	int is_increasing = 1;
	double prev, curr;
	CellPtr curr_cons = args;
	CellPtr curr_car = curr_cons->get_car()->eval();
	if(curr_car->is_int())
		prev = curr_car->get_int();
	else
		prev = curr_car->get_double();
	curr_cons = curr_cons->get_cdr();
	while(!curr_cons->is_nil()){
		curr_car = curr_cons->get_car()->eval();
		if(curr_car->is_int())
			curr = curr_car->get_int();
		else
			curr = curr_car->get_double();
		if(prev >= curr){
			is_increasing = 0;
			// check whether the remaining operands are valid or not;
			curr_cons = curr_cons->get_cdr();
			while(!curr_cons->is_nil()){
				curr_car = curr_cons->get_car()->eval();
				if(!(curr_car->is_int() || curr_car->is_double()))
					throw runtime_error("try to use < operator with a Cell that cannot do");
				curr_cons = curr_cons->get_cdr();
			}
			break;
		}
		prev = curr;
		curr_cons = curr_cons->get_cdr();
	}
	return make_shared<IntCell>(is_increasing);
}

CellPtr PrimitiveProcedureCell::ceiling(CellPtr const args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("ceiling operator requires exactly one double operand");
	CellPtr operand = args->get_car()->eval();
	if(operand->is_double()){
		double d = operand->get_double();
		int i = static_cast<int>(d);
		if(d > 0 && d > i)
			++i;
		return make_shared<IntCell>(i);
	}
	else
		throw runtime_error("try to use ceiling operator with non-double Cell");
}

CellPtr PrimitiveProcedureCell::pri_floor(CellPtr const args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("floor operator requires exactly one double operand");
	CellPtr operand = args->get_car()->eval();
	if(operand->is_double()){
		double d = operand->get_double();
		int i = static_cast<int>(d);
		if(d < 0 && d < i)
			--i;
		return make_shared<IntCell>(i);
	}
	else
		throw runtime_error("try to use floor operator with non-double Cell");
}

CellPtr PrimitiveProcedureCell::ifelse(CellPtr const args)
{
	if(args->is_nil() || args->get_cdr()->is_nil() 
	   || !(args->get_cdr()->get_cdr()->is_nil() || args->get_cdr()->get_cdr()->get_cdr()->is_nil()))
		throw runtime_error("if operator requires either two or three operands");
	CellPtr curr_cons = args;
	CellPtr condition_cell = curr_cons->get_car()->eval();
	if(condition_cell->is_int() && condition_cell->get_int() == 0
	   || condition_cell->is_double() && condition_cell->get_double() == 0){

		CellPtr false_cell = curr_cons->get_cdr()->get_cdr();
		if(false_cell->is_nil())
			return smart_nil;
		else
			return false_cell->get_car()->eval();
	}
	else
		return curr_cons->get_cdr()->get_car()->eval();
}

CellPtr PrimitiveProcedureCell::quote(CellPtr const args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("quote operator requires exactly one operand");
	return args->get_car();
}

CellPtr PrimitiveProcedureCell::cons(CellPtr const args)
{
	if(args->is_nil() || args->get_cdr()->is_nil() || !args->get_cdr()->get_cdr()->is_nil())
		throw runtime_error("cons operator requires exactly two operands");
	CellPtr car = args->get_car()->eval();
	CellPtr cdr = args->get_cdr()->get_car()->eval();
	if(!(cdr->is_nil() || cdr->is_cons()))
		throw runtime_error("cdr of a cons must be a list");
	return make_shared<ConsCell>(car, cdr);
}

CellPtr PrimitiveProcedureCell::car(CellPtr const args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("car operator requires exactly one operand");
	CellPtr operand = args->get_car()->eval();
	if(operand->is_cons())
		return operand->get_car();
	else
		throw runtime_error("car operator requires exactly one list operand");
}

CellPtr PrimitiveProcedureCell::cdr(CellPtr const args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("cdr operator requires exactly one operand");
	CellPtr operand = args->get_car()->eval();
	if(operand->is_cons())
		return operand->get_cdr();
	else
		throw runtime_error("cdr operator requires exactly one list operand");
}

CellPtr PrimitiveProcedureCell::nullp(CellPtr const args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("nullp operator requires exactly one operand");
	CellPtr operand = args->get_car()->eval();
	return make_shared<IntCell>(operand->is_nil());
}

CellPtr PrimitiveProcedureCell::intp(CellPtr const args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("intp operator requires exactly one operand");
	CellPtr operand = args->get_car()->eval();
	return make_shared<IntCell>(operand->is_int());
}

CellPtr PrimitiveProcedureCell::doublep(CellPtr const args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("doublep operator requires exactly one operand");
	CellPtr operand = args->get_car()->eval();
	return make_shared<IntCell>(operand->is_double());
}

CellPtr PrimitiveProcedureCell::symbolp(CellPtr const args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("symbolp operator requires exactly one operand");
	CellPtr operand = args->get_car()->eval();
	return make_shared<IntCell>(operand->is_symbol());
}

CellPtr PrimitiveProcedureCell::listp(CellPtr const args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("listp operator requires exactly one operand");
	CellPtr operand = args->get_car()->eval();
	return make_shared<IntCell>(operand->is_nil() || operand->is_cons());
}

CellPtr PrimitiveProcedureCell::pri_not(CellPtr const args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("not operator requires exactly one operand");
	CellPtr operand = args->get_car()->eval();
	if(operand->is_int() && operand->get_int() == 0 || operand->is_double() && operand->get_double() == 0)
		return make_shared<IntCell>(1);
	else
		return make_shared<IntCell>(0);
}

CellPtr PrimitiveProcedureCell::define(CellPtr const args)
{
	if(args->is_nil() || args->get_cdr()->is_nil() || !args->get_cdr()->get_cdr()->is_nil())
		throw runtime_error("define operator requires exactly two operands");
	string s = args->get_car()->get_symbol();
	if(symbol_table.begin()->find(s) != symbol_table.begin()->end())
		throw runtime_error("symbol \"" + s + "\" cannot be re-defined in this scope");
	else
		symbol_table.begin()->insert(map<string, CellPtr>::value_type(s, args->get_cdr()->get_car()->eval()));
	return smart_nil;
}

CellPtr PrimitiveProcedureCell::pri_print(CellPtr const args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("print operator requires exactly one operand");
	args->get_car()->eval()->print();
	cout << endl;
	return smart_nil;
}

CellPtr PrimitiveProcedureCell::pri_eval(CellPtr const args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("eval operator requires exactly one operand");
	return args->get_car()->eval()->eval();
}

CellPtr PrimitiveProcedureCell::lambda(CellPtr const args)
{
	if(args->is_nil() || args->get_cdr()->is_nil())
		throw runtime_error("lambda operator requires two or more operands");
	CellPtr formals = args->get_car();
	CellPtr body = args->get_cdr();
	if(formals->is_symbol()){
		//throw runtime_error("variable number of arguments is not supported in the current version");
	}
	else{
		set<string> arg_set; // use to check duplicate argument name
		string s;
		CellPtr curr_cons = formals;
		while(!curr_cons->is_nil()){
			try{
				s = curr_cons->get_car()->get_symbol();
			}catch(const runtime_error& e){
				throw runtime_error("the arguments list contains invalid identifier(s)");
			}
			if(arg_set.insert(s).second == false)
				throw runtime_error("duplicate argument name: \"" + s + "\"");
			curr_cons = curr_cons->get_cdr();
		}
	}
	return make_shared<ProcedureCell>(formals, body);
}

CellPtr PrimitiveProcedureCell::pri_apply(CellPtr const args)
{
	if(args->is_nil() || args->get_cdr()->is_nil() || !args->get_cdr()->get_cdr()->is_nil())
		throw runtime_error("apply operator requires exactly two operands");
	CellPtr procedure = args->get_car()->eval();
	CellPtr arguments = args->get_cdr()->get_car()->eval();
	return procedure->apply(arguments);
}

CellPtr PrimitiveProcedureCell::let(CellPtr const args)
{
	if(args->is_nil() || args->get_cdr()->is_nil())
		throw runtime_error("let operator requires two or more operands");
	CellPtr formals = smart_nil, arguments = smart_nil, body = args->get_cdr(),
		curr_cons = args->get_car(), curr_pair;
	set<string> arg_set;
	string s;
	while(!curr_cons->is_nil()){
		curr_pair = curr_cons->get_car();
		if(curr_pair->is_nil() || curr_pair->get_cdr()->is_nil() || !curr_pair->get_cdr()->get_cdr()->is_nil())
			throw runtime_error("bad syntax");
		s = curr_pair->get_car()->get_symbol();
		if(arg_set.insert(s).second == false)
			throw runtime_error("duplicate argument name: \"" + s + "\"");
		formals = make_shared<ConsCell>(make_shared<SymbolCell>(s), formals);
		arguments = make_shared<ConsCell>(curr_pair->get_cdr()->get_car(), arguments);
		curr_cons = curr_cons->get_cdr();
	}
	return ProcedureCell(formals, body).apply(arguments);
}

template <typename IntOp, typename DoubleOp>
CellPtr PrimitiveProcedureCell::arithmetic_operation(CellPtr const operands, IntOp int_op, DoubleOp double_op, const std::string& op)
{
	int int_result = 0;
	double double_result = 0;
	CellPtr curr_operand = operands->get_car()->eval();
	CellPtr curr_cons = operands->get_cdr();
	if(curr_operand->is_int()){
		int_result = curr_operand->get_int();
		while(!curr_cons->is_nil()){
			curr_operand = curr_cons->get_car()->eval();
			curr_cons = curr_cons->get_cdr();
			if(curr_operand->is_int())
				int_result = int_op(int_result, curr_operand->get_int());
			else if(curr_operand->is_double()){
				double_result = double_op(int_result, curr_operand->get_double());
				if(curr_cons->is_nil())
					return make_shared<DoubleCell>(double_result);
				break;
			}
			else
				throw runtime_error("operant for " + op + " is neither an int nor a double");
		}
		if(curr_cons->is_nil())
			return make_shared<IntCell>(int_result);
	}
	else if(curr_operand->is_double())
		double_result = curr_operand->get_double();
	else
		throw runtime_error("operant for " + op + " is neither an int nor a double");

	while(!curr_cons->is_nil()){
		curr_operand = curr_cons->get_car()->eval();
		curr_cons = curr_cons->get_cdr();
		if(curr_operand->is_int())
			double_result = double_op(double_result, curr_operand->get_int());
		else if(curr_operand->is_double())
			double_result = double_op(double_result, curr_operand->get_double());
		else
			throw runtime_error("operant for " + op + " is neither an int nor a double");
	}
	return make_shared<DoubleCell>(double_result);
}

CellPtr PrimitiveProcedureCell::pri_set(CellPtr const args)
{
	if(args->is_nil() || args->get_cdr()->is_nil() || !args->get_cdr()->get_cdr()->is_nil())
		throw runtime_error("set! operator requires exactly two operands");
	string s = args->get_car()->get_symbol();
	map<string, CellPtr>::iterator it = search_table(s);
	if(it == symbol_table.rbegin()->end())
		throw runtime_error("cannot set undefined variable \"" + s + "\"");
	else
		it->second = args->get_cdr()->get_car()->eval();
	return smart_nil;	
}

CellPtr PrimitiveProcedureCell::pri_exit(CellPtr const args)
{
	if(!(args->is_nil() || args->get_cdr()->is_nil()))
		throw runtime_error("exit requires either 0 or 1 operand");
	if(!args->is_nil()){
		CellPtr c = args->get_car()->eval();
		if(c->is_int())
			exit(c->get_int());
	}
	else
		exit(0);
}

void readfile(ifstream& fin); // defined in main.cpp
CellPtr PrimitiveProcedureCell::loadfile(CellPtr const args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("load operator requires exactly one operand");
	if(args->get_car()->is_symbol()){
		string filename = args->get_car()->get_symbol();
		ifstream file(filename.c_str());
		if(file.is_open())
			readfile(file);
		else
			throw runtime_error("cannot open file \"" + filename + "\"");
		return smart_nil;
	}
	else
		throw runtime_error("load operator requires exactly one symbol operand");
}

map<string, CellPtr> PrimitiveProcedureCell::create_map()
{
	map<string, CellPtr> init_map;
	init_map.insert(pair<string, CellPtr>("+", make_shared<PrimitiveProcedureCell>(&add)));
	init_map.insert(pair<string, CellPtr>("-", make_shared<PrimitiveProcedureCell>(&subtract)));
	init_map.insert(pair<string, CellPtr>("*", make_shared<PrimitiveProcedureCell>(&multiply)));
	init_map.insert(pair<string, CellPtr>("/", make_shared<PrimitiveProcedureCell>(&divide)));
	init_map.insert(pair<string, CellPtr>("<", make_shared<PrimitiveProcedureCell>(&less_than)));
	init_map.insert(pair<string, CellPtr>("ceiling", make_shared<PrimitiveProcedureCell>(&ceiling)));
	init_map.insert(pair<string, CellPtr>("floor", make_shared<PrimitiveProcedureCell>(&pri_floor)));
	init_map.insert(pair<string, CellPtr>("if", make_shared<PrimitiveProcedureCell>(&ifelse)));
	init_map.insert(pair<string, CellPtr>("quote", make_shared<PrimitiveProcedureCell>(&quote)));
	init_map.insert(pair<string, CellPtr>("cons", make_shared<PrimitiveProcedureCell>(&cons)));
	init_map.insert(pair<string, CellPtr>("car", make_shared<PrimitiveProcedureCell>(&car)));
	init_map.insert(pair<string, CellPtr>("cdr", make_shared<PrimitiveProcedureCell>(&cdr)));
	init_map.insert(pair<string, CellPtr>("nullp", make_shared<PrimitiveProcedureCell>(&nullp)));
	init_map.insert(pair<string, CellPtr>("intp", make_shared<PrimitiveProcedureCell>(&intp)));
	init_map.insert(pair<string, CellPtr>("doublep", make_shared<PrimitiveProcedureCell>(&doublep)));
	init_map.insert(pair<string, CellPtr>("symbolp", make_shared<PrimitiveProcedureCell>(&symbolp)));
	init_map.insert(pair<string, CellPtr>("listp", make_shared<PrimitiveProcedureCell>(&listp)));
	init_map.insert(pair<string, CellPtr>("not", make_shared<PrimitiveProcedureCell>(&pri_not)));
	init_map.insert(pair<string, CellPtr>("define", make_shared<PrimitiveProcedureCell>(&define)));
	init_map.insert(pair<string, CellPtr>("print", make_shared<PrimitiveProcedureCell>(&pri_print)));
	init_map.insert(pair<string, CellPtr>("eval", make_shared<PrimitiveProcedureCell>(&pri_eval)));
	init_map.insert(pair<string, CellPtr>("lambda", make_shared<PrimitiveProcedureCell>(&lambda)));
	init_map.insert(pair<string, CellPtr>("apply", make_shared<PrimitiveProcedureCell>(&pri_apply)));
	init_map.insert(pair<string, CellPtr>("let", make_shared<PrimitiveProcedureCell>(&let)));
	init_map.insert(pair<string, CellPtr>("set!", make_shared<PrimitiveProcedureCell>(&pri_set)));
	init_map.insert(pair<string, CellPtr>("exit", make_shared<PrimitiveProcedureCell>(&pri_exit)));
	init_map.insert(pair<string, CellPtr>("load", make_shared<PrimitiveProcedureCell>(&loadfile)));
	return init_map;
}
