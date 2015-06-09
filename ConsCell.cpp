#include "ConsCell.hpp"
#include "IntCell.hpp"
#include "DoubleCell.hpp"
#include "SymbolCell.hpp"
#include <functional> // plus, minus, multiplies, divides
#include <map>
#include <iterator>
using namespace std;

map<string, Cell*> symbol_table;

ConsCell::ConsCell(Cell* car, Cell* cdr):car_m(car), cdr_m(cdr)
{

}

ConsCell::~ConsCell()
{
	delete car_m;
	delete cdr_m;
}

bool ConsCell::is_int() const
{
	return false;
}

bool ConsCell::is_double() const
{
	return false;
}

bool ConsCell::is_symbol() const
{
	return false;
}

bool ConsCell::is_cons() const
{
	return true;
}

bool ConsCell::is_nil() const
{
	return false;
}

int ConsCell::get_int() const
{
	throw runtime_error("try to access the int member of a non-int Cell");
}

double ConsCell::get_double() const
{
	throw runtime_error("try to access the double member of a non-double Cell");
}

std::string ConsCell::get_symbol() const
{
	throw runtime_error("try to access the symbol member of a non-symbol Cell");
}

Cell* ConsCell::get_car() const
{
	return car_m;
}

Cell* ConsCell::get_cdr() const
{
	return cdr_m;
}

Cell* ConsCell::eval()
{
	//Cell* curr_car = get_car()->eval();
	Cell* curr_car = get_car();
	// curr_car must be a symbol cell, otherwise not a valid expression
	if(curr_car->get_symbol() == "+"){
		if(get_cdr()->is_nil())
			return new IntCell(0);
		else
			return arithmetic_operation(get_cdr(), plus<int>(), plus<double>(), "+");
	}
	else if(curr_car->get_symbol() == "-"){
		if(get_cdr()->is_nil())
			throw runtime_error("- operator requires at least one operand");
		else if(get_cdr()->get_cdr()->is_nil()){
			if(get_cdr()->get_car()->is_int())
				return new IntCell(0 - get_cdr()->get_car()->get_int());
			else if(get_cdr()->get_car()->is_double())
				return new DoubleCell(0 - get_cdr()->get_car()->get_double());
			else
				throw runtime_error("operant for - is neither an int nor a double");
		}
		else
			return arithmetic_operation(get_cdr(), minus<int>(), minus<double>(), "-");
	}
	else if(curr_car->get_symbol() == "*"){
		if(get_cdr()->is_nil())
			return new IntCell(1);
		else
			return arithmetic_operation(get_cdr(), multiplies<int>(), multiplies<double>(), "*");
	}
	else if(curr_car->get_symbol() == "/"){
		if(get_cdr()->is_nil())
			throw runtime_error("/ operator requires at least one operand");
		else if(get_cdr()->get_cdr()->is_nil()){
			if(get_cdr()->get_car()->is_int())
				return new IntCell(1 / get_cdr()->get_car()->get_int());
			else if(get_cdr()->get_car()->is_double())
				return new DoubleCell(1.0 / get_cdr()->get_car()->get_double());
			else
				throw runtime_error("operant for / is neither an int nor a double");
		}
		else
			return arithmetic_operation(get_cdr(), divides<int>(), divides<double>(), "/");
	}
	else if(curr_car->get_symbol() == "if"){
		Cell* curr_cons = get_cdr();
		if(curr_cons->is_nil() || curr_cons->get_cdr()->is_nil() 
			|| !(curr_cons->get_cdr()->get_cdr()->is_nil() 
				|| curr_cons->get_cdr()->get_cdr()->get_cdr()->is_nil()))
			throw runtime_error("if operator requires either two or three operands");
		Cell* condition_cell = curr_cons->get_car()->eval();
		if(condition_cell->is_int() && condition_cell->get_int()
			|| condition_cell->is_double() && condition_cell->get_double()
			|| condition_cell->is_symbol() && !condition_cell->get_symbol().empty())
			return curr_cons->get_cdr()->get_car()->eval();
		else{
			Cell* false_cell = curr_cons->get_cdr()->get_cdr();
			if(false_cell->is_nil())
				return nil;
			else
				return false_cell->get_car()->eval();
		}
	}
	else if(curr_car->get_symbol() == "ceiling"){
		if(get_cdr()->is_nil() || !get_cdr()->get_cdr()->is_nil())
			throw runtime_error("ceiling operator requires exactly one double operand");
		Cell* operand = get_cdr()->get_car()->eval();
		if(operand->is_double()){
			double d = operand->get_double();
			int i = static_cast<int>(d);
			if(d > 0 && d > i)
				++i;
			return new IntCell(i);
		}
		else
			throw runtime_error("try to use ceiling operator with non-double Cell");
	}
	else if(curr_car->get_symbol() == "floor"){
		if(get_cdr()->is_nil() || !get_cdr()->get_cdr()->is_nil())
			throw runtime_error("floor operator requires exactly one double operand");
		Cell* operand = get_cdr()->get_car()->eval();
		if(operand->is_double()){
			double d = operand->get_double();
			int i = static_cast<int>(d);
			if(d < 0 && d < i)
				--i;
			return new IntCell(i);
		}
		else
			throw runtime_error("try to use floor operator with non-double Cell");
	}
	else if(curr_car->get_symbol() == "quote"){
		if(get_cdr()->is_nil() || !get_cdr()->get_cdr()->is_nil())
			throw runtime_error("quote operator requires exactly one operand");
		return get_cdr()->get_car();
	}
	else if(curr_car->get_symbol() == "cons"){
		if(get_cdr()->is_nil() || !(get_cdr()->get_cdr()->is_nil() || get_cdr()->get_cdr()->get_cdr()->is_nil()))
			throw runtime_error("cons operator requires exactly two operands");
		Cell* car = get_cdr()->get_car()->eval();
		Cell* cdr = get_cdr()->get_cdr()->get_car()->eval();
		return new ConsCell(car, cdr);
	}
	else if(curr_car->get_symbol() == "car"){
		if(get_cdr()->is_nil() || !get_cdr()->get_cdr()->is_nil())
			throw runtime_error("car operator requires exactly one operand");
		Cell* operand = get_cdr()->get_car()->eval();
		if(operand->is_cons())
			return operand->get_car();
		else
			throw runtime_error("car operator requires exactly one list operand");
	}
	else if(curr_car->get_symbol() == "cdr"){
		if(get_cdr()->is_nil() || !get_cdr()->get_cdr()->is_nil())
			throw runtime_error("cdr operator requires exactly one operand");
		Cell* operand = get_cdr()->get_car()->eval();
		if(operand->is_cons())
			return operand->get_cdr();
		else
			throw runtime_error("cdr operator requires exactly one list operand");
	}
	else if(curr_car->get_symbol() == "nullp"){
		if(get_cdr()->is_nil() || !get_cdr()->get_cdr()->is_nil())
			throw runtime_error("nullp operator requires exactly one operand");
		Cell* operand = get_cdr()->get_car()->eval();
		return new IntCell(operand->is_nil());
	}
	else if(curr_car->get_symbol() == "define"){
		if(get_cdr()->is_nil() || !(get_cdr()->get_cdr()->is_nil() || get_cdr()->get_cdr()->get_cdr()->is_nil()))
			throw runtime_error("define operator requires exactly two operands");
		string s = get_cdr()->get_car()->get_symbol();
		if(symbol_table.find(s) != symbol_table.end())
			throw runtime_error("symbol " + s + " cannot be re-defined");
		else
			symbol_table.insert(map<string, Cell*>::value_type(s, get_cdr()->get_cdr()->get_car()->eval()));
		return nil;
	}
	else if(curr_car->get_symbol() == "print"){
		if(get_cdr()->is_nil() || !get_cdr()->get_cdr()->is_nil())
			throw runtime_error("print operator requires exactly one operand");
		get_cdr()->get_car()->eval()->print();
		cout << endl;
		return nil;
	}
	else if(curr_car->get_symbol() == "eval"){
		if(get_cdr()->is_nil() || !get_cdr()->get_cdr()->is_nil())
			throw runtime_error("eval operator requires exactly one operand");
		return get_cdr()->get_car()->eval()->eval();
	}
	else if(curr_car->get_symbol() == "not"){
		if(get_cdr()->is_nil() || !get_cdr()->get_cdr()->is_nil())
			throw runtime_error("not operator requires exactly one operand");
		Cell* operand = get_cdr()->get_car()->eval();
		if(operand->is_int() && operand->get_int() == 0 || operand->is_double() && operand->get_double() == 0)
			return new IntCell(1);
		else
			return new IntCell(0);
	}
	else if(curr_car->get_symbol() == "<"){
		if(get_cdr()->is_nil())
			return new IntCell(1);
		int is_increasing = 1;
		double prev, curr;
		Cell* curr_cons = get_cdr();
		Cell* curr_car = curr_cons->get_car()->eval();
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
				break;
			}
			prev = curr;
			curr_cons = curr_cons->get_cdr();
		}
		return new IntCell(is_increasing);
	}
	else
		throw runtime_error(curr_car->get_symbol() + string(" is not an operator"));
}

template <typename IntOp, typename DoubleOp>
Cell* ConsCell::arithmetic_operation(Cell* const operands, IntOp int_op, DoubleOp double_op, const std::string& op)
{
	int int_result = 0;
	double double_result = 0;
	Cell* curr_operand = operands->get_car()->eval();
	Cell* curr_cons = operands->get_cdr();
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
					return new DoubleCell(double_result);
				break;
			}
			else
				throw runtime_error("operant for " + op + " is neither an int nor a double");
		}
		if(curr_cons->is_nil())
			return new IntCell(int_result);
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
	return new DoubleCell(double_result);
}

void ConsCell::print(ostream& os) const
{
	os << "(";
	const Cell* current_cell = this;
	while(true){
		current_cell->get_car()->print(os);
		if(current_cell->get_cdr()->is_nil())
			break;
		os << " ";
		current_cell = current_cell->get_cdr();
	}
	os << ")";
}
