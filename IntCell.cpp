#include "errmsg.hpp"
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
	errmsg("try to access the double member of a non-double Cell");
}

std::string IntCell::get_symbol() const
{
	errmsg("try to access the symbol member of a non-symbol Cell");
}

Cell* IntCell::get_car() const
{
	errmsg("try to access the car member of a non-cons Cell");
}

Cell* IntCell::get_cdr() const
{
	errmsg("try to access the cdr member of a non-cons Cell");
}

Cell* IntCell::eval()
{
	return this;
}

void IntCell::print(ostream& os) const
{
	os << int_m;
}
