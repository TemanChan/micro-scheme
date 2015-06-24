#ifndef PROCEDURECELL_HPP
#define PROCEDURECELL_HPP

#include "Cell.hpp"

class ProcedureCell:public Cell
{
public:
	ProcedureCell(CellPtr formals, CellPtr body, ScopePtr parent_scope = ScopePtr());
	virtual bool is_procedure() const;
	virtual CellPtr get_formals() const;
	virtual CellPtr get_body() const;
	virtual void print(std::ostream& os = std::cout) const;
	virtual CellPtr apply(CellPtr const args);

private:
	CellPtr formals_m;
	CellPtr body_m;
	ScopePtr parent_scope_m;
};

#endif
