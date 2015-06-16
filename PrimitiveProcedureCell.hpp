#ifndef PRIMITIVEPROCEDURECELL_HPP
#define PRIMITIVEPROCEDURECELL_HPP

#include "ProcedureCell.hpp"
#include <iostream>
#include <string>
#include <map>

class PrimitiveProcedureCell:public ProcedureCell
{
public:
	PrimitiveProcedureCell(Cell* (*func)(const Cell* const));
	virtual Cell* clone() const;
	virtual ~PrimitiveProcedureCell();
	virtual Cell* apply(const Cell* const args) const;
	virtual void print(std::ostream& os = std::cout) const;
	static Cell* add(const Cell* const args);
	static Cell* subtract(const Cell* const args);
	static Cell* multiply(const Cell* const args);
	static Cell* divide(const Cell* const args);
	static Cell* less_than(const Cell* const args);
	static Cell* ceiling(const Cell* const args);
	static Cell* pri_floor(const Cell* const args);
	static Cell* ifelse(const Cell* const args);
	static Cell* quote(const Cell* const args);
	static Cell* cons(const Cell* const args);
	static Cell* car(const Cell* const args);
	static Cell* cdr(const Cell* const args);
	static Cell* nullp(const Cell* const args);
	static Cell* pri_not(const Cell* const args);
	static Cell* define(const Cell* const args);
	static Cell* pri_print(const Cell* const args);
	static Cell* pri_eval(const Cell* const args);
	static Cell* lambda(const Cell* const args);
	static Cell* pri_apply(const Cell* const args);
	static Cell* let(const Cell* const args);
	static std::map<std::string, Cell*> create_map();

private:
	template <typename IntOp, typename DoubleOp>
	static Cell* arithmetic_operation(const Cell* const operands, IntOp int_op, DoubleOp double_op, const std::string& op);
	Cell* (*func_m)(const Cell* const);
};

#endif
