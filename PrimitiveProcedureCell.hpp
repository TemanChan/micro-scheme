#ifndef PRIMITIVEPROCEDURECELL_HPP
#define PRIMITIVEPROCEDURECELL_HPP

#include "ProcedureCell.hpp"
#include <iostream>
#include <string>
#include <map>

class PrimitiveProcedureCell:public ProcedureCell
{
public:
	PrimitiveProcedureCell(Cell* (*func)(Cell* const));
	virtual ~PrimitiveProcedureCell();
	virtual Cell* apply(Cell* const args);
	virtual void print(std::ostream& os = std::cout) const;
	static Cell* add(Cell* const args);
	static Cell* subtract(Cell* const args);
	static Cell* multiply(Cell* const args);
	static Cell* divide(Cell* const args);
	static Cell* less_than(Cell* const args);
	static Cell* ceiling(Cell* const args);
	static Cell* pri_floor(Cell* const args);
	static Cell* ifelse(Cell* const args);
	static Cell* quote(Cell* const args);
	static Cell* cons(Cell* const args);
	static Cell* car(Cell* const args);
	static Cell* cdr(Cell* const args);
	static Cell* nullp(Cell* const args);
	static Cell* pri_not(Cell* const args);
	static Cell* define(Cell* const args);
	static Cell* pri_print(Cell* const args);
	static Cell* pri_eval(Cell* const args);
	static Cell* lambda(Cell* const args);
	static Cell* pri_apply(Cell* const args);
	static Cell* let(Cell* const args);
	static std::map<std::string, Cell*> create_map();

private:
	template <typename IntOp, typename DoubleOp>
	static Cell* arithmetic_operation(Cell* const operands, IntOp int_op, DoubleOp double_op, const std::string& op);
	Cell* (*func_m)(Cell* const);
};

#endif
