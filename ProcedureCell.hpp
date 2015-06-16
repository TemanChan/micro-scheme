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
	ProcedureCell(Cell* formals, Cell* body);
	virtual Cell* clone() const;
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
	virtual const Cell* get_car() const;
	virtual const Cell* get_cdr() const;
	virtual const Cell* get_formals() const;
	virtual const Cell* get_body() const;
	virtual void print(std::ostream& os = std::cout) const;
	virtual Cell* eval() const;
	virtual Cell* apply(const Cell* const args) const;

private:
	Cell* formals_m;
	Cell* body_m;
};

#endif
