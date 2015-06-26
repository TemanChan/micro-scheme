#ifndef PROCEDURECELL_HPP
#define PROCEDURECELL_HPP

#include "Cell.hpp"

class ProcedureCell:public Cell
{
public:
	ProcedureCell(CellPtr formals, CellPtr body, ScopePtr parent_scope);
	ProcedureCell(CellPtr formals, CellPtr body, ScopeWPtr parent_scope = ScopeWPtr());
	virtual bool is_procedure() const;
	virtual CellPtr get_formals() const;
	virtual CellPtr get_body() const;
	virtual void print(std::ostream& os = std::cout) const;
	virtual CellPtr apply(const CellPtr& args);

private:
	CellPtr formals_m;
	CellPtr body_m;
	ScopePtr parent_scope_sp;
	ScopeWPtr parent_scope_wp;
};

class PrimitiveProcedureCell:public ProcedureCell
{
public:
	PrimitiveProcedureCell(CellPtr (*func)(const CellPtr&));
	virtual CellPtr apply(const CellPtr& args);
	virtual void print(std::ostream& os = std::cout) const;

private:
	CellPtr (*func_m)(const CellPtr&);
};

#endif
