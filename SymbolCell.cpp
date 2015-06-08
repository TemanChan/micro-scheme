#include "SymbolCell.hpp"
#include "errmsg.hpp"
using namespace std;

Cell* const nil = new SymbolCell("");

SymbolCell::SymbolCell(const string& s):symbol_m(s)
{

}

SymbolCell::~SymbolCell()
{

}

bool SymbolCell::is_int() const
{
	return false;
}

bool SymbolCell::is_double() const
{
	return false;
}

bool SymbolCell::is_symbol() const
{
	return this != nil;
}

bool SymbolCell::is_cons() const
{
	return false;
}

bool SymbolCell::is_nil() const
{
	return this == nil;
}

int SymbolCell::get_int() const
{
	errmsg("try to access the int member of a non-int Cell");
}

double SymbolCell::get_double() const
{
	errmsg("try to access the double member of a non-double Cell");
}

std::string SymbolCell::get_symbol() const
{
	if(this == nil)
		errmsg("try to access the symbol member of a non-symbol Cell");
	return symbol_m;
}

Cell* SymbolCell::get_car() const
{
	errmsg("try to access the car member of a non-cons Cell");
}

Cell* SymbolCell::get_cdr() const
{
	errmsg("try to access the cdr member of a non-cons Cell");
}

Cell* SymbolCell::eval()
{
	if(this == nil)
		errmsg("empty list cannot be evaluated");
	return this;
}

void SymbolCell::print(std::ostream& os) const
{
	os << symbol_m;
}
