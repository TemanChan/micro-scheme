#ifndef PRIMITIVEPROCEDURECELL_HPP
#define PRIMITIVEPROCEDURECELL_HPP

#include "ProcedureCell.hpp"

class PrimitiveProcedureCell:public ProcedureCell
{
public:
	PrimitiveProcedureCell(CellPtr (*func)(CellPtr const));
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
	static CellPtr intp(CellPtr const args);
	static CellPtr doublep(CellPtr const args);
	static CellPtr symbolp(CellPtr const args);
	static CellPtr listp(CellPtr const args);
	static CellPtr pri_not(CellPtr const args);
	static CellPtr define(CellPtr const args);
	static CellPtr pri_print(CellPtr const args);
	static CellPtr pri_eval(CellPtr const args);
	static CellPtr lambda(CellPtr const args);
	static CellPtr pri_apply(CellPtr const args);
	static CellPtr let(CellPtr const args);
	static CellPtr pri_set(CellPtr const args);
	static CellPtr pri_exit(CellPtr const args);
	static CellPtr loadfile(CellPtr const args);
	static std::map<std::string, CellPtr> create_map();

private:
	template <typename IntOp, typename DoubleOp>
	static CellPtr arithmetic_operation(CellPtr const operands, IntOp int_op, DoubleOp double_op, const std::string& op);
	static CellPtr num_lt(double prev, CellPtr curr_cons);
	static CellPtr symbol_lt(std::string prev, CellPtr curr_cons);
	CellPtr (*func_m)(CellPtr const);
};

#endif
