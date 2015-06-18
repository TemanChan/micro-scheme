#ifndef PRIMITIVEPROCEDURECELL_HPP
#define PRIMITIVEPROCEDURECELL_HPP

#include "ProcedureCell.hpp"
#include <iostream>
#include <string>
#include <map>

class PrimitiveProcedureCell:public ProcedureCell
{
public:
	PrimitiveProcedureCell(CellPtr (*func)(CellPtr const));
	virtual ~PrimitiveProcedureCell();
	virtual CellPtr apply(CellPtr const args);
	virtual void print(std::ostream& os = std::cout) const;
	static CellPtr add(CellPtr const args);
	static CellPtr subtract(CellPtr const args);
	static CellPtr multiply(CellPtr const args);
	static CellPtr divide(CellPtr const args);
	static CellPtr less_than(CellPtr const args);
	static CellPtr ceiling(CellPtr const args);
	static CellPtr pri_floor(CellPtr const args);
	static CellPtr ifelse(CellPtr const args);
	static CellPtr quote(CellPtr const args);
	static CellPtr cons(CellPtr const args);
	static CellPtr car(CellPtr const args);
	static CellPtr cdr(CellPtr const args);
	static CellPtr nullp(CellPtr const args);
	static CellPtr pri_not(CellPtr const args);
	static CellPtr define(CellPtr const args);
	static CellPtr pri_print(CellPtr const args);
	static CellPtr pri_eval(CellPtr const args);
	static CellPtr lambda(CellPtr const args);
	static CellPtr pri_apply(CellPtr const args);
	static CellPtr let(CellPtr const args);
	static CellPtr pri_set(CellPtr const args);
	static std::map<std::string, CellPtr> create_map();

private:
	template <typename IntOp, typename DoubleOp>
	static CellPtr arithmetic_operation(CellPtr const operands, IntOp int_op, DoubleOp double_op, const std::string& op);
	CellPtr (*func_m)(CellPtr const);
};

#endif
