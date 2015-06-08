#include "ConsCell.hpp"
#include "IntCell.hpp"
#include "DoubleCell.hpp"
#include "SymbolCell.hpp"
#include "errmsg.hpp"
#include <functional> // plus, minus, multiplies, divides
using namespace std;

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
	errmsg("try to access the int member of a non-int Cell");
}

double ConsCell::get_double() const
{
	errmsg("try to access the double member of a non-double Cell");
}

std::string ConsCell::get_symbol() const
{
	errmsg("try to access the symbol member of a non-symbol Cell");
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
	Cell* curr_car = get_car()->eval();
	// curr_car must be a symbol cell, otherwise not a valid expression
	if(curr_car->get_symbol() == "+"){
		if(get_cdr()->is_nil())
			return new IntCell(0);
		else
			return arithmetic_operation(get_cdr(), plus<int>(), plus<double>(), "+");
	}
	else if(curr_car->get_symbol() == "-"){
		if(get_cdr()->is_nil())
			errmsg("- operator requires at least one operand");
		else if(get_cdr()->get_cdr()->is_nil()){
			if(get_cdr()->get_car()->is_int())
				return new IntCell(0 - get_cdr()->get_car()->get_int());
			else if(get_cdr()->get_car()->is_double())
				return new DoubleCell(0 - get_cdr()->get_car()->get_double());
			else
				errmsg("operant for - is neither an int nor a double");
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
			errmsg("/ operator requires at least one operand");
		else if(get_cdr()->get_cdr()->is_nil()){
			if(get_cdr()->get_car()->is_int())
				return new IntCell(1 / get_cdr()->get_car()->get_int());
			else if(get_cdr()->get_car()->is_double())
				return new DoubleCell(1.0 / get_cdr()->get_car()->get_double());
			else
				errmsg("operant for / is neither an int nor a double");
		}
		else
			return arithmetic_operation(get_cdr(), divides<int>(), divides<double>(), "/");
	}
	else if(curr_car->get_symbol() == "if"){
		Cell* curr_cons = get_cdr();
		if(curr_cons->is_nil() || curr_cons->get_cdr()->is_nil() 
			|| !(curr_cons->get_cdr()->get_cdr()->is_nil() 
				|| curr_cons->get_cdr()->get_cdr()->get_cdr()->is_nil()))
			errmsg("if operator requires either two or three operands");
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
			errmsg("ceiling operator requires exactly one double operand");
		Cell* operand = get_cdr()->get_car()->eval();
		if(operand->is_double()){
			double d = operand->get_double();
			int i = static_cast<int>(d);
			if(d > 0 && d > i)
				++i;
			return new IntCell(i);
		}
		else
			errmsg("try to use ceiling operator with non-double Cell");
	}
	else if(curr_car->get_symbol() == "floor"){
		if(get_cdr()->is_nil() || !get_cdr()->get_cdr()->is_nil())
			errmsg("floor operator requires exactly one double operand");
		Cell* operand = get_cdr()->get_car()->eval();
		if(operand->is_double()){
			double d = operand->get_double();
			int i = static_cast<int>(d);
			if(d < 0 && d < i)
				--i;
			return new IntCell(i);
		}
		else
			errmsg("try to use floor operator with non-double Cell");
	}
	else if(curr_car->get_symbol() == "quote"){
		if(get_cdr()->is_nil() || !get_cdr()->get_cdr()->is_nil())
			errmsg("quote operator requires exactly one operand");
		return get_cdr()->get_car();
	}
	else if(curr_car->get_symbol() == "cons"){
		if(get_cdr()->is_nil() || !(get_cdr()->get_cdr()->is_nil() || get_cdr()->get_cdr()->get_cdr()->is_nil()))
			errmsg("cons operator requires exactly two operands");
		Cell* car = get_cdr()->get_car()->eval();
		Cell* cdr = get_cdr()->get_cdr()->get_car()->eval();
		return new ConsCell(car, cdr);
	}
	else if(curr_car->get_symbol() == "car"){
		if(get_cdr()->is_nil() || !get_cdr()->get_cdr()->is_nil())
			errmsg("car operator requires exactly one operand");
		Cell* operand = get_cdr()->get_car()->eval();
		if(operand->is_cons())
			return operand->get_car();
		else
			errmsg("car operator requires exactly one list operand");
	}
	else if(curr_car->get_symbol() == "cdr"){
		if(get_cdr()->is_nil() || !get_cdr()->get_cdr()->is_nil())
			errmsg("cdr operator requires exactly one operand");
		Cell* operand = get_cdr()->get_car()->eval();
		if(operand->is_cons())
			return operand->get_cdr();
		else
			errmsg("cdr operator requires exactly one list operand");
	}
	else if(curr_car->get_symbol() == "nullp"){
		if(get_cdr()->is_nil() || !get_cdr()->get_cdr()->is_nil())
			errmsg("nullp operator requires exactly one operand");
		Cell* operand = get_cdr()->get_car()->eval();
		return new IntCell(operand->is_nil());
	}
	else
		errmsg(curr_car->get_symbol() + string(" is not an operator"));
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
				errmsg("operant for " + op + " is neither an int nor a double");
		}
		if(curr_cons->is_nil())
			return new IntCell(int_result);
	}
	else if(curr_operand->is_double())
		double_result = curr_operand->get_double();
	else
		errmsg("operant for " + op + " is neither an int nor a double");

	while(!curr_cons->is_nil()){
		curr_operand = curr_cons->get_car()->eval();
		curr_cons = curr_cons->get_cdr();
		if(curr_operand->is_int())
			double_result = double_op(double_result, curr_operand->get_int());
		else if(curr_operand->is_double())
			double_result = double_op(double_result, curr_operand->get_double());
		else
			errmsg("operant for " + op + " is neither an int nor a double");
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
