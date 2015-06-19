#include "Cell.hpp"
using namespace std;

bool Cell::is_int() const
{
	return false;
}

bool Cell::is_double() const
{
	return false;
}

bool Cell::is_symbol() const
{
	return false;
}

bool Cell::is_cons() const
{
	return false;
}

bool Cell::is_procedure() const
{
	return false;
}

bool Cell::is_nil() const
{
	return false;
}

int Cell::get_int() const
{
	throw runtime_error("try to access the int member of a non-int Cell");
}

double Cell::get_double() const
{
	throw runtime_error("try to access the double member of a non-double Cell");
}

std::string Cell::get_symbol() const
{
	throw runtime_error("try to access the symbol member of a non-symbol Cell");
}

CellPtr Cell::get_car() const
{
	throw runtime_error("try to access the car member of a non-cons Cell");
}

CellPtr Cell::get_cdr() const
{
	throw runtime_error("try to access the cdr member of a non-cons Cell");
}

CellPtr Cell::get_formals() const
{
	throw runtime_error("try to access the formals member of a non-procedure Cell");
}

CellPtr Cell::get_body() const
{
	throw runtime_error("try to access the body member of a non-procedure Cell");
}

CellPtr Cell::eval()
{
	return shared_from_this();
}

CellPtr Cell::apply(CellPtr const args)
{
	throw runtime_error("try to apply with a Cell that cannot do");
}


Cell* const nil = new SymbolCell("()");
CellPtr const smart_nil(nil);
list<map<string, CellPtr> > symbol_table(1, PrimitiveProcedureCell::create_map());

map<string, CellPtr>::iterator search_table(const string& s)
{
	list<map<string, CellPtr> >::iterator i = symbol_table.begin();
	map<string, CellPtr>::iterator j;
	for(; i != symbol_table.end(); ++i){
		j = i->find(s);
		if(j != i->end())
			return j;
	}
	return j;
}
