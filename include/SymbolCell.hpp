#ifndef SYMBOLCELL_HPP
#define SYMBOLCELL_HPP

#include "Cell.hpp"

class SymbolCell:public Cell
{
public:
	SymbolCell(const std::string& s);
	virtual bool is_symbol() const;
	virtual bool is_nil() const;
	virtual std::string get_symbol() const;
	virtual void print(std::ostream& os = std::cout) const;
	virtual CellPtr eval();
private:
	std::string symbol_m;
};

#endif
