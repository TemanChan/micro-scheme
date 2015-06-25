#ifndef BUILTINMACROCELL_HPP
#define BUILTINMACROCELL_HPP

#include "Cell.hpp"

class BuiltinMacroCell:public Cell
{
public:
	BuiltinMacroCell(CellPtr (*func)(const CellPtr&));
	virtual bool is_macro() const;
	virtual CellPtr transform_eval(const CellPtr& args);
	virtual void print(std::ostream& os = std::cout) const;

private:
	CellPtr (*func_m)(const CellPtr&);
};

#endif
