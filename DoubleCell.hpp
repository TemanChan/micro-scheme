#ifndef DOUBLECELL_H
#define DOUBLECELL_H

#include "Cell.hpp"

class DoubleCell:public Cell
{
public:
	DoubleCell(double d);
	virtual Cell* clone() const;
	virtual ~DoubleCell();
	virtual bool is_int() const;
	virtual bool is_double() const;
	virtual bool is_symbol() const;
	virtual bool is_cons() const;
	virtual bool is_procedure() const;
	virtual bool is_nil() const;
	virtual int get_int() const;
	virtual double get_double() const;
	virtual std::string get_symbol() const;
	virtual const Cell* get_car() const;
	virtual const Cell* get_cdr() const;
	virtual const Cell* get_formals() const;
	virtual const Cell* get_body() const;
	virtual void print(std::ostream& os = std::cout) const;
	virtual Cell* eval() const;
	virtual Cell* apply(const Cell* const args) const;

private:
	double double_m;
};

#endif
