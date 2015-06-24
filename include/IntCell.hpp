#ifndef INTCELL_HPP
#define INTCELL_HPP

#include "Cell.hpp"

class IntCell:public Cell
{
public:
	IntCell(int i);
	virtual bool is_int() const;
	virtual int get_int() const;
	virtual void print(std::ostream& os = std::cout) const;
private:
	int int_m;
};

#endif
