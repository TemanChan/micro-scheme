#include "BuiltinMacroCell.hpp"

BuiltinMacroCell::BuiltinMacroCell(CellPtr (*func)(const CellPtr&)):func_m(func)
{

}

bool BuiltinMacroCell::is_macro() const
{
	return true;
}

CellPtr BuiltinMacroCell::transform_eval(const CellPtr& args)
{
	return func_m(args);
}

void BuiltinMacroCell::print(std::ostream& os) const
{
	os << "#<built-in macro>";
}
