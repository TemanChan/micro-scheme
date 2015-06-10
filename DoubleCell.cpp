#include "DoubleCell.hpp"
using namespace std;

DoubleCell::DoubleCell(double d):double_m(d)
{

}

DoubleCell::~DoubleCell()
{

}

bool DoubleCell::is_int() const
{
	return false;
}

bool DoubleCell::is_double() const
{
	return true;
}

bool DoubleCell::is_symbol() const
{
	return false;
}

bool DoubleCell::is_cons() const
{
	return false;
}

bool DoubleCell::is_procedure() const
{
	return false;
}

bool DoubleCell::is_nil() const
{
	return false;
}

int DoubleCell::get_int() const
{
	throw runtime_error("try to access the int member of a non-int Cell");
}

double DoubleCell::get_double() const
{
	return double_m;
}

std::string DoubleCell::get_symbol() const
{
	throw runtime_error("try to access the symbol member of a non-symbol Cell");
}

Cell* DoubleCell::get_car() const
{
	throw runtime_error("try to access the car member of a non-cons Cell");
}

Cell* DoubleCell::get_cdr() const
{
	throw runtime_error("try to access the cdr member of a non-cons Cell");
}

Cell* DoubleCell::get_formals() const
{
	throw runtime_error("try to access the formals member of a non-procedure Cell");
}

Cell* DoubleCell::get_body() const
{
	throw runtime_error("try to access the body member of a non-procedure Cell");
}

Cell* DoubleCell::apply(Cell* const args)
{
	throw runtime_error("try to apply with a double Cell");
}

Cell* DoubleCell::eval()
{
	return this;
}

void DoubleCell::print(ostream& os) const
{
	/*
	os.fill('0');
	os.width(7);
	os.setf(ios::showpoint);
	os << left << double_m
	*/
	os.precision(6);
	os << fixed << double_m;
}
