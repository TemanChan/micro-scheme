#include "Cell.hpp"
using namespace std;

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
	map<string, CellPtr>::iterator it = search_table(symbol_m);
	if(it == symbol_table.rbegin()->end())
		throw runtime_error("try to evaluate an un-definded symbol Cell \"" + symbol_m + "\"");
	else
		return it->second;
}

void SymbolCell::print(std::ostream& os) const
{
	os << symbol_m;
}
