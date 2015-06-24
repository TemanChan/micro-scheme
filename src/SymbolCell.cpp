#include "SymbolCell.hpp"
#include "Scope.hpp"
using namespace std;

Cell* const nil = new SymbolCell("()");
CellPtr const smart_nil(nil);

SymbolCell::SymbolCell(const string& s):symbol_m(s)
{

}

bool SymbolCell::is_symbol() const
{
	return this != nil;
}

bool SymbolCell::is_nil() const
{
	return this == nil;
}

std::string SymbolCell::get_symbol() const
{
	if(this == nil)
		throw runtime_error("try to access the symbol member of a non-symbol Cell");
	return symbol_m;
}

CellPtr SymbolCell::eval()
{
	if(this == nil)
		throw runtime_error("empty list cannot be evaluated");
	return current_scope->eval(symbol_m);
}

void SymbolCell::print(std::ostream& os) const
{
	os << symbol_m;
}
