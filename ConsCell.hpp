#ifndef CONSCELL_H
#define CONSCELL_H

#include "Cell.hpp"

class ConsCell:public Cell
{
public:
	ConsCell(Cell* car, Cell* cdr);
	virtual ~ConsCell();
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
	/*
	Cell* plus(Cell* c);
	Cell* minus(Cell* c);
	Cell* multiply(Cell* c);
	Cell* divide(Cell* c);
	Cell* evalIf(Cell* c);
	Cell* evalCeiling(Cell* c);
	*/
	template <typename IntOp, typename DoubleOp>
	Cell* arithmetic_operation(Cell* const operands, IntOp int_op, DoubleOp double_op, const std::string& op);
	Cell* car_m;
	Cell* cdr_m;
};

#endif
