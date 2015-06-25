#include "BuiltinMacro.hpp"

BuiltinMacro::BuiltinMacro(CellPtr (*func)(const CellPtr&)):func_m(func)
{

}

bool BuiltinMacro::is_macro() const
{
	return true;
}

CellPtr BuiltinMacro::transform_eval(const CellPtr& args)
{
	return func_m(args);
}

void BuiltinMacro::print(std::ostream& os) const
{
	os << "#<built-in macro>";
}
