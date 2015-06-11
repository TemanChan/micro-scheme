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
	virtual bool is_procedure() const;
	virtual bool is_nil() const;
	virtual int get_int() const;
	virtual double get_double() const;
	virtual std::string get_symbol() const;
	virtual Cell* get_car() const;
	virtual Cell* get_cdr() const;
	virtual Cell* get_formals() const;
	virtual Cell* get_body() const;
	virtual void print(std::ostream& os = std::cout) const;
	virtual Cell* eval();
	virtual Cell* apply(Cell* const args);

private:
	Cell* car_m;
	Cell* cdr_m;
};

#endif
