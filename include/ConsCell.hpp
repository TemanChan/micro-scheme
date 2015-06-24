#ifndef CONSCELL_HPP
#define CONSCELL_HPP

#include "Cell.hpp"

class ConsCell:public Cell
{
public:
	ConsCell(CellPtr car, CellPtr cdr);
	virtual bool is_cons() const;
	virtual CellPtr get_car() const;
	virtual CellPtr get_cdr() const;
	virtual void print(std::ostream& os = std::cout) const;
	virtual CellPtr eval();

private:
	CellPtr car_m;
	CellPtr cdr_m;
};

#endif
