#include "IntCell.hpp"
using namespace std;


IntCell::IntCell(int i):int_m(i)
{

}

IntCell::~IntCell()
{

}

bool IntCell::is_int() const
{
	return true;
}

bool IntCell::is_double() const
{
	return false;
}

bool IntCell::is_symbol() const
{
	return false;
}

bool IntCell::is_cons() const
{
	return false;
}

bool IntCell::is_procedure() const
{
	return false;
}

bool IntCell::is_nil() const
{
	return false;
}

int IntCell::get_int() const
{
	return int_m;
}

double IntCell::get_double() const
{
	throw runtime_error("try to access the double member of a non-double Cell");
}

std::string IntCell::get_symbol() const
{
	throw runtime_error("try to access the symbol member of a non-symbol Cell");
}

Cell* IntCell::get_car() const
{
	throw runtime_error("try to access the car member of a non-cons Cell");
}

Cell* IntCell::get_cdr() const
{
	throw runtime_error("try to access the cdr member of a non-cons Cell");
}

Cell* IntCell::get_formals() const
{
	throw runtime_error("try to access the formals member of a non-procedure Cell");
}

Cell* IntCell::get_body() const
{
	throw runtime_error("try to access the body member of a non-procedure Cell");
}

Cell* IntCell::apply(Cell* const args)
{
	throw runtime_error("try to apply with an int Cell");
}

Cell* IntCell::eval()
{
	return this;
}

void IntCell::print(ostream& os) const
{
	os << int_m;
}
