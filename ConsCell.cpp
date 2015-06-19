#include "Cell.hpp"
using namespace std;

ConsCell::ConsCell(CellPtr car, CellPtr cdr):car_m(car), cdr_m(cdr)
{

}

bool ConsCell::is_cons() const
{
	return true;
}

CellPtr ConsCell::get_car() const
{
	return car_m;
}

CellPtr ConsCell::get_cdr() const
{
	return cdr_m;
}

CellPtr ConsCell::eval()
{
	return get_car()->eval()->apply(get_cdr());
}

void ConsCell::print(ostream& os) const
{
	os << "(";
	shared_ptr<const Cell> current_cell = shared_from_this();
	while(true){
		current_cell->get_car()->print(os);
		if(current_cell->get_cdr()->is_nil())
			break;
		os << " ";
		current_cell = current_cell->get_cdr();
	}
	os << ")";
}
