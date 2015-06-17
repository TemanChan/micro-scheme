#ifndef PROCEDURECELL_HPP
#define PROCEDURECELL_HPP

#include "Cell.hpp"

class ProcedureCell:public Cell
{
public:
	/**
	 * @brief Check if this is an int cell.
	 * @return True iff this is an int cell.
	 */
	ProcedureCell(CellPtr formals, CellPtr body);
	virtual ~ProcedureCell();
	virtual bool is_int() const;
	virtual bool is_double() const;
	virtual bool is_symbol() const;
	virtual bool is_cons() const;
	virtual bool is_procedure() const;
	virtual bool is_nil() const;
	virtual int get_int() const;
	virtual double get_double() const;
	virtual std::string get_symbol() const;
	virtual CellPtr get_car() const;
	virtual CellPtr get_cdr() const;
	virtual CellPtr get_formals() const;
	virtual CellPtr get_body() const;
	virtual void print(std::ostream& os = std::cout) const;
	virtual CellPtr eval();
	virtual CellPtr apply(CellPtr const args);

private:
	CellPtr formals_m;
	CellPtr body_m;
};

#endif
