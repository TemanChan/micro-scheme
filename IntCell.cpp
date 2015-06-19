#include "Cell.hpp"
using namespace std;

IntCell::IntCell(int i):int_m(i)
{

}

bool IntCell::is_int() const
{
	return true;
}

int IntCell::get_int() const
{
	return int_m;
}

void IntCell::print(ostream& os) const
{
	os << int_m;
}
