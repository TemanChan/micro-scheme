#include "SymbolCell.hpp"
using namespace std;

Cell* const nil = new SymbolCell("()");
list<map<string, Cell*>> symbol_table;

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

bool SymbolCell::is_procedure() const
{
	return false;
}

bool SymbolCell::is_nil() const
{
	return this == nil;
}

int SymbolCell::get_int() const
{
	throw runtime_error("try to access the int member of a non-int Cell");
}

double SymbolCell::get_double() const
{
	throw runtime_error("try to access the double member of a non-double Cell");
}

std::string SymbolCell::get_symbol() const
{
	if(this == nil)
		throw runtime_error("try to access the symbol member of a non-symbol Cell");
	return symbol_m;
}

Cell* SymbolCell::get_car() const
{
	throw runtime_error("try to access the car member of a non-cons Cell");
}

Cell* SymbolCell::get_cdr() const
{
	throw runtime_error("try to access the cdr member of a non-cons Cell");
}

Cell* SymbolCell::get_formals() const
{
	throw runtime_error("try to access the formals member of a non-procedure Cell");
}

Cell* SymbolCell::get_body() const
{
	throw runtime_error("try to access the body member of a non-procedure Cell");
}

Cell* SymbolCell::apply(Cell* const args)
{
	throw runtime_error("try to apply with a symbol Cell");
}

Cell* SymbolCell::eval()
{
	if(this == nil)
		throw runtime_error("empty list cannot be evaluated");
	map<string, Cell*>::iterator it = search_table(symbol_m);
	if(it == symbol_table.rbegin().end())
		throw runtime_error("attempt to evaluate un-definded symbol Cell " + symbol_m);
	else
		return it->second;
}

void SymbolCell::print(std::ostream& os) const
{
	os << symbol_m;
}
