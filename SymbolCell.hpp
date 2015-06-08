#ifndef SYMBOLCELL_H
#define SYMBOLCELL_H

#include "Cell.hpp"
#include <string>

class SymbolCell:public Cell
{
public:
	SymbolCell(const std::string& s);
	virtual ~SymbolCell();
	virtual bool is_int() const;
	virtual bool is_double() const;
	virtual bool is_symbol() const;
	virtual bool is_cons() const;
	virtual bool is_nil() const;
	virtual int get_int() const;
	virtual double get_double() const;
	virtual std::string get_symbol() const;
	virtual Cell* get_car() const;
	virtual Cell* get_cdr() const;
	virtual void print(std::ostream& os = std::cout) const;
	virtual Cell* eval();
private:
	std::string symbol_m;
};

#endif
