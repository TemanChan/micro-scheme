#include "Cell.hpp"
using namespace std;

DoubleCell::DoubleCell(double d):double_m(d)
{

}

bool DoubleCell::is_double() const
{
	return true;
}

double DoubleCell::get_double() const
{
	return double_m;
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
