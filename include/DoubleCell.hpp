#ifndef DOUBLECELL_HPP
#define DOUBLECELL_HPP

#include "Cell.hpp"

class DoubleCell:public Cell
{
public:
	DoubleCell(double d);
	virtual bool is_double() const;
	virtual double get_double() const;
	virtual void print(std::ostream& os = std::cout) const;

private:
	double double_m;
};

#endif
