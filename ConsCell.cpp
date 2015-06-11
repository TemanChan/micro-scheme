#include "ConsCell.hpp"
#include "IntCell.hpp"
#include "DoubleCell.hpp"
#include "SymbolCell.hpp"
#include "ProcedureCell.hpp"
#include "PrimitiveProcedureCell.hpp"
using namespace std;

ConsCell::ConsCell(Cell* car, Cell* cdr):car_m(car), cdr_m(cdr)
{

}

ConsCell::~ConsCell()
{
	delete car_m;
	delete cdr_m;
}

bool ConsCell::is_int() const
{
	return false;
}

bool ConsCell::is_double() const
{
	return false;
}

bool ConsCell::is_symbol() const
{
	return false;
}

bool ConsCell::is_cons() const
{
	return true;
}

bool ConsCell::is_procedure() const
{
	return false;
}

bool ConsCell::is_nil() const
{
	return false;
}

int ConsCell::get_int() const
{
	throw runtime_error("try to access the int member of a non-int Cell");
}

double ConsCell::get_double() const
{
	throw runtime_error("try to access the double member of a non-double Cell");
}

string ConsCell::get_symbol() const
{
	throw runtime_error("try to access the symbol member of a non-symbol Cell");
}

Cell* ConsCell::get_car() const
{
	return car_m;
}

Cell* ConsCell::get_cdr() const
{
	return cdr_m;
}

Cell* ConsCell::get_formals() const
{
	throw runtime_error("try to access the formals member of a non-procedure Cell");
}

Cell* ConsCell::get_body() const
{
	throw runtime_error("try to access the body member of a non-procedure Cell");
}

Cell* ConsCell::apply(Cell* const args)
{
	throw runtime_error("try to apply with a cons Cell");
}

Cell* ConsCell::eval()
{
	return get_car()->eval()->apply(get_cdr());
}

void ConsCell::print(ostream& os) const
{
	os << "(";
	const Cell* current_cell = this;
	while(true){
		current_cell->get_car()->print(os);
		if(current_cell->get_cdr()->is_nil())
			break;
		os << " ";
		current_cell = current_cell->get_cdr();
	}
	os << ")";
}
