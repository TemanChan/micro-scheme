#include "ProcedureCell.hpp"
using namespace std;

bool ProcedureCell::is_int() const
{
	return false;
}

bool ProcedureCell::is_double() const
{
	return false;
}

bool ProcedureCell::is_symbol() const
{
	return false;
}

bool ProcedureCell::is_cons() const
{
	return false;
}

bool ProcedureCell::is_procedure() const
{
	return true;
}

bool ProcedureCell::is_nil() const
{
	return false;
}

int ProcedureCell::get_int() const
{
	throw runtime_error("try to access the int member of a non-int Cell");
}

double ProcedureCell::get_double() const
{
	throw runtime_error("try to access the double member of a non-double Cell");
}

string ProcedureCell::get_symbol() const
{
	throw runtime_error("try to access the symbol member of a non-symbol Cell");
}

Cell* ProcedureCell::get_car() const
{
	throw runtime_error("try to access the car member of a non-cons Cell");
}

Cell* ProcedureCell::get_cdr() const
{
	throw runtime_error("try to access the cdr member of a non-cons Cell");
}

Cell* ProcedureCell::get_formals() const
{
	return formals_m;
}

Cell* ProcedureCell::get_body() const
{
	return body_m;
}

void ProcedureCell::print(ostream& os) const
{
	os << "a procedure Cell";
}

Cell* ProcedureCell::eval()
{
	return this;
}

Cell* ProcedureCell::apply(Cell* const args)
{
	
}
