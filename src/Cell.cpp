#include <functional>
#include <set>
#include <cstdlib> // exit
#include <fstream>
#include "Cell.hpp"
#include "IntCell.hpp"
#include "DoubleCell.hpp"
#include "SymbolCell.hpp"
#include "ConsCell.hpp"
#include "ProcedureCell.hpp"
#include "BuiltinMacroCell.hpp"
#include "Scope.hpp"
using namespace std;

bool Cell::is_int() const
{
	return false;
}

bool Cell::is_double() const
{
	return false;
}

bool Cell::is_symbol() const
{
	return false;
}

bool Cell::is_cons() const
{
	return false;
}

bool Cell::is_procedure() const
{
	return false;
}

bool Cell::is_nil() const
{
	return false;
}

bool Cell::is_macro() const
{
	return false;
}

int Cell::get_int() const
{
	throw runtime_error("try to access the int member of a non-int Cell");
}

double Cell::get_double() const
{
	throw runtime_error("try to access the double member of a non-double Cell");
}

std::string Cell::get_symbol() const
{
	throw runtime_error("try to access the symbol member of a non-symbol Cell");
}

CellPtr Cell::get_car() const
{
	throw runtime_error("try to access the car member of a non-cons Cell");
}

CellPtr Cell::get_cdr() const
{
	throw runtime_error("try to access the cdr member of a non-cons Cell");
}

CellPtr Cell::get_formals() const
{
	throw runtime_error("try to access the formals member of a non-procedure Cell");
}

CellPtr Cell::get_body() const
{
	throw runtime_error("try to access the body member of a non-procedure Cell");
}

CellPtr Cell::eval()
{
	return shared_from_this();
}

CellPtr Cell::apply(const CellPtr& args)
{
	throw runtime_error("try to apply with a Cell that cannot do");
}

CellPtr Cell::transform_eval(const CellPtr& args)
{
	throw runtime_error("try to transform with a non-macro Cell");
}

// global functions
// built-in macros
CellPtr ifelse(const CellPtr& args)
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

CellPtr quote(const CellPtr& args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("quote operator requires exactly one operand");
	return args->get_car();
}

CellPtr define(const CellPtr& args)
{
	if(args->is_nil() || args->get_cdr()->is_nil() || !args->get_cdr()->get_cdr()->is_nil())
		throw runtime_error("define operator requires exactly two operands");
	string s = args->get_car()->get_symbol();
	if(current_scope->count(s))
		throw runtime_error("symbol \"" + s + "\" cannot be re-defined in this scope");
	else
		current_scope->insert(Scope::value_type(s, args->get_cdr()->get_car()->eval()));
	return smart_nil;
}

CellPtr lambda(const CellPtr& args)
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
	if(is_sp_needed)
		return make_shared<ProcedureCell>(formals, body, current_scope);
	else
		return make_shared<ProcedureCell>(formals, body, ScopeWPtr(current_scope));
}

CellPtr let(const CellPtr& args)
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
	if(is_sp_needed)
		return ProcedureCell(formals, body, current_scope).apply(arguments);
	else
		return ProcedureCell(formals, body, ScopeWPtr(current_scope)).apply(arguments);
}

CellPtr macro_set(const CellPtr& args)
{
	if(args->is_nil() || args->get_cdr()->is_nil() || !args->get_cdr()->get_cdr()->is_nil())
		throw runtime_error("set! operator requires exactly two operands");
	string s = args->get_car()->get_symbol();
	map<string, CellPtr>::iterator it = current_scope->find(s);
	if(it == current_scope->end())
		throw runtime_error("cannot set undefined variable \"" + s + "\"");
	else
		it->second = args->get_cdr()->get_car()->eval();
	return smart_nil;	
}

void readfile(const char *fn); // defined in main.cpp
CellPtr loadfile(const CellPtr& args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("load operator requires exactly one operand");
	if(args->get_car()->is_symbol()){
		string filename = args->get_car()->get_symbol();
		readfile(filename.c_str());
		return smart_nil;
	}
	else
		throw runtime_error("load operator requires exactly one symbol operand");
}

// primitive procedures
template <typename IntOp, typename DoubleOp>
CellPtr arithmetic_operation(const CellPtr& operands, IntOp int_op, DoubleOp double_op, const std::string& op)
{
	int int_result = 0;
	double double_result = 0;
	CellPtr curr_operand = operands->get_car();
	CellPtr curr_cons = operands->get_cdr();
	if(curr_operand->is_int()){
		int_result = curr_operand->get_int();
		while(!curr_cons->is_nil()){
			curr_operand = curr_cons->get_car();
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
		curr_operand = curr_cons->get_car();
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

CellPtr add(const CellPtr& args)
{
	if(args->is_nil())
		return make_shared<IntCell>(0);
	else
		return arithmetic_operation(args, plus<int>(), plus<double>(), "+");
}

CellPtr subtract(const CellPtr& args)
{
	if(args->is_nil())
		throw runtime_error("- operator requires at least one operand");
	else if(args->get_cdr()->is_nil()){
		CellPtr operand = args->get_car();
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

CellPtr multiply(const CellPtr& args)
{
	if(args->is_nil())
		return make_shared<IntCell>(1);
	else
		return arithmetic_operation(args, multiplies<int>(), multiplies<double>(), "*");
}

CellPtr divide(const CellPtr& args)
{
	if(args->is_nil())
		throw runtime_error("/ operator requires at least one operand");
	else if(args->get_cdr()->is_nil()){
		CellPtr operand = args->get_car();
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

CellPtr num_lt(double prev, CellPtr curr_cons)
{
	int is_increasing = 1;
	double curr;
	CellPtr curr_car = smart_nil;
	while(!curr_cons->is_nil()){
		curr_car = curr_cons->get_car();
		if(curr_car->is_int())
			curr = curr_car->get_int();
		else
			curr = curr_car->get_double();
		if(prev >= curr){
			is_increasing = 0;
			// check whether the remaining operands are valid or not;
			curr_cons = curr_cons->get_cdr();
			while(!curr_cons->is_nil()){
				curr_car = curr_cons->get_car();
				if(!(curr_car->is_int() || curr_car->is_double()))
					throw runtime_error("operands for operator < are invalid");
				curr_cons = curr_cons->get_cdr();
			}
			break;
		}
		prev = curr;
		curr_cons = curr_cons->get_cdr();
	}
	return make_shared<IntCell>(is_increasing);
}

CellPtr symbol_lt(string prev, CellPtr curr_cons)
{
	int is_increasing = 1;
	string curr;
	CellPtr curr_car = smart_nil;
	while(!curr_cons->is_nil()){
		curr_car = curr_cons->get_car();
		curr = curr_car->get_symbol();
		if(prev >= curr){
			is_increasing = 0;
			// check whether the remaining operands are valid or not;
			curr_cons = curr_cons->get_cdr();
			while(!curr_cons->is_nil()){
				curr_car = curr_cons->get_car();
				if(!curr_car->is_symbol())
					throw runtime_error("operands for operator < are invalid");
				curr_cons = curr_cons->get_cdr();
			}
			break;
		}
		prev = curr;
		curr_cons = curr_cons->get_cdr();
	}
	return make_shared<IntCell>(is_increasing);
}

CellPtr less_than(const CellPtr& args)
{
	if(args->is_nil())
		return make_shared<IntCell>(1);
	CellPtr operand = args->get_car();
	if(operand->is_int())
		return num_lt(operand->get_int(), args->get_cdr());
	else if(operand->is_double())
		return num_lt(operand->get_double(), args->get_cdr());
	else if(operand->is_symbol())
		return symbol_lt(operand->get_symbol(), args->get_cdr());
	else
		throw runtime_error("try to use < operator with a Cell that cannot do");
}

CellPtr ceiling(const CellPtr& args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("ceiling operator requires exactly one double operand");
	CellPtr operand = args->get_car();
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

CellPtr pri_floor(const CellPtr& args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("floor operator requires exactly one double operand");
	CellPtr operand = args->get_car();
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

CellPtr cons(const CellPtr& args)
{
	if(args->is_nil() || args->get_cdr()->is_nil() || !args->get_cdr()->get_cdr()->is_nil())
		throw runtime_error("cons operator requires exactly two operands");
	CellPtr car = args->get_car();
	CellPtr cdr = args->get_cdr()->get_car();
	if(!(cdr->is_nil() || cdr->is_cons()))
		throw runtime_error("cdr of a cons must be a list");
	return make_shared<ConsCell>(car, cdr);
}

CellPtr car(const CellPtr& args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("car operator requires exactly one operand");
	CellPtr operand = args->get_car();
	if(operand->is_cons())
		return operand->get_car();
	else
		throw runtime_error("car operator requires exactly one list operand");
}

CellPtr cdr(const CellPtr& args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("cdr operator requires exactly one operand");
	CellPtr operand = args->get_car();
	if(operand->is_cons())
		return operand->get_cdr();
	else
		throw runtime_error("cdr operator requires exactly one list operand");
}

CellPtr nullp(const CellPtr& args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("nullp operator requires exactly one operand");
	return make_shared<IntCell>(args->get_car()->is_nil());
}

CellPtr intp(const CellPtr& args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("intp operator requires exactly one operand");
	return make_shared<IntCell>(args->get_car()->is_int());
}

CellPtr doublep(const CellPtr& args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("doublep operator requires exactly one operand");
	return make_shared<IntCell>(args->get_car()->is_double());
}

CellPtr symbolp(const CellPtr& args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("symbolp operator requires exactly one operand");
	return make_shared<IntCell>(args->get_car()->is_symbol());
}

CellPtr listp(const CellPtr& args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("listp operator requires exactly one operand");
	return make_shared<IntCell>(args->get_car()->is_nil() || args->get_car()->is_cons());
}

CellPtr pri_not(const CellPtr& args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("not operator requires exactly one operand");
	CellPtr operand = args->get_car();
	if(operand->is_int() && operand->get_int() == 0 || operand->is_double() && operand->get_double() == 0)
		return make_shared<IntCell>(1);
	else
		return make_shared<IntCell>(0);
}

CellPtr pri_print(const CellPtr& args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("print operator requires exactly one operand");
	args->get_car()->print();
	cout << endl;
	return smart_nil;
}

CellPtr pri_eval(const CellPtr& args)
{
	if(args->is_nil() || !args->get_cdr()->is_nil())
		throw runtime_error("eval operator requires exactly one operand");
	return args->get_car()->eval();
}

CellPtr pri_apply(const CellPtr& args)
{
	if(args->is_nil() || args->get_cdr()->is_nil() || !args->get_cdr()->get_cdr()->is_nil())
		throw runtime_error("apply operator requires exactly two operands");
	CellPtr procedure = args->get_car();
	CellPtr arguments = args->get_cdr()->get_car();
	return procedure->apply(arguments);
}

CellPtr pri_exit(const CellPtr& args)
{
	if(!(args->is_nil() || args->get_cdr()->is_nil()))
		throw runtime_error("exit requires either 0 or 1 operand");
	if(!args->is_nil()){
		CellPtr c = args->get_car();
		if(c->is_int())
			exit(c->get_int());
	}
	else
		exit(0);
}

ostream &operator<<(ostream &os, const Cell &cell){
	cell.print(os);
	return os;
}

map<string, CellPtr> create_map()
{
	map<string, CellPtr> init_map;
	init_map.insert(pair<string, CellPtr>("if", make_shared<BuiltinMacroCell>(&ifelse)));
	init_map.insert(pair<string, CellPtr>("quote", make_shared<BuiltinMacroCell>(&quote)));
	init_map.insert(pair<string, CellPtr>("define", make_shared<BuiltinMacroCell>(&define)));
	init_map.insert(pair<string, CellPtr>("set!", make_shared<BuiltinMacroCell>(&macro_set)));
	init_map.insert(pair<string, CellPtr>("lambda", make_shared<BuiltinMacroCell>(&lambda)));
	init_map.insert(pair<string, CellPtr>("let", make_shared<BuiltinMacroCell>(&let)));
	init_map.insert(pair<string, CellPtr>("load", make_shared<BuiltinMacroCell>(&loadfile)));
	init_map.insert(pair<string, CellPtr>("+", make_shared<PrimitiveProcedureCell>(&add)));
	init_map.insert(pair<string, CellPtr>("-", make_shared<PrimitiveProcedureCell>(&subtract)));
	init_map.insert(pair<string, CellPtr>("*", make_shared<PrimitiveProcedureCell>(&multiply)));
	init_map.insert(pair<string, CellPtr>("/", make_shared<PrimitiveProcedureCell>(&divide)));
	init_map.insert(pair<string, CellPtr>("<", make_shared<PrimitiveProcedureCell>(&less_than)));
	init_map.insert(pair<string, CellPtr>("ceiling", make_shared<PrimitiveProcedureCell>(&ceiling)));
	init_map.insert(pair<string, CellPtr>("floor", make_shared<PrimitiveProcedureCell>(&pri_floor)));
	init_map.insert(pair<string, CellPtr>("cons", make_shared<PrimitiveProcedureCell>(&cons)));
	init_map.insert(pair<string, CellPtr>("car", make_shared<PrimitiveProcedureCell>(&car)));
	init_map.insert(pair<string, CellPtr>("cdr", make_shared<PrimitiveProcedureCell>(&cdr)));
	init_map.insert(pair<string, CellPtr>("nullp", make_shared<PrimitiveProcedureCell>(&nullp)));
	init_map.insert(pair<string, CellPtr>("intp", make_shared<PrimitiveProcedureCell>(&intp)));
	init_map.insert(pair<string, CellPtr>("doublep", make_shared<PrimitiveProcedureCell>(&doublep)));
	init_map.insert(pair<string, CellPtr>("symbolp", make_shared<PrimitiveProcedureCell>(&symbolp)));
	init_map.insert(pair<string, CellPtr>("listp", make_shared<PrimitiveProcedureCell>(&listp)));
	init_map.insert(pair<string, CellPtr>("not", make_shared<PrimitiveProcedureCell>(&pri_not)));
	init_map.insert(pair<string, CellPtr>("print", make_shared<PrimitiveProcedureCell>(&pri_print)));
	init_map.insert(pair<string, CellPtr>("eval", make_shared<PrimitiveProcedureCell>(&pri_eval)));
	init_map.insert(pair<string, CellPtr>("apply", make_shared<PrimitiveProcedureCell>(&pri_apply)));
	init_map.insert(pair<string, CellPtr>("exit", make_shared<PrimitiveProcedureCell>(&pri_exit)));
	return init_map;
}

bool is_sp_needed = false;
ScopePtr global_scope = make_shared<Scope>(ScopePtr(), create_map());
ScopePtr current_scope = global_scope;
