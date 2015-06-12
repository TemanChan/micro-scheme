#include "ProcedureCell.hpp"
#include "ConsCell.hpp"
#include <stack>
using namespace std;

ProcedureCell::ProcedureCell(Cell* formals, Cell* body):formals_m(formals), body_m(body)
{

}

ProcedureCell::~ProcedureCell()
{
	if(formals_m != nil)
		delete formals_m;
	if(body_m != nil)
		delete body_m;
}

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
	os << "#<procedure>";
}

Cell* ProcedureCell::eval()
{
	return this;
}

Cell* ProcedureCell::apply(Cell* const args)
{
	map<string, Cell*> local_table;
	if(formals_m->is_symbol()){
		stack<Cell*> arg_stack;
		Cell* curr_cons = args;
		while(!curr_cons->is_nil()){
			arg_stack.push(curr_cons->get_car()->eval());
			curr_cons = curr_cons->get_cdr();
		}
		
		Cell* arguments = nil;
		while(!arg_stack.empty()){
			arguments = new ConsCell(arg_stack.top(), arguments);
			arg_stack.pop();
		}
		local_table.insert(map<string, Cell*>::value_type(formals_m->get_symbol(), arguments));
	}
	else{
		Cell* form_cons = formals_m;
		Cell* arg_cons = args;
		while(!(form_cons->is_nil() || arg_cons->is_nil())){
			local_table.insert(map<string, Cell*>::value_type(form_cons->get_car()->get_symbol(), arg_cons->get_car()->eval()));
			form_cons = form_cons->get_cdr();
			arg_cons = arg_cons->get_cdr();
		}

		if(!(form_cons->is_nil() && arg_cons->is_nil()))
			throw runtime_error("arguments number mismatch");
	}

	symbol_table.push_front(local_table);

	Cell* body_cons = body_m;
	while(!body_cons->get_cdr()->is_nil()){
		body_cons->get_car()->eval();
		body_cons = body_cons->get_cdr();
	}
	Cell* result = body_cons->get_car()->eval();
	symbol_table.pop_front();
	return result;
}
