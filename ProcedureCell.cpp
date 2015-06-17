#include "ProcedureCell.hpp"
#include "ConsCell.hpp"
#include <stack>
using namespace std;

ProcedureCell::ProcedureCell(CellPtr formals, CellPtr body):formals_m(formals), body_m(body)
{

}

ProcedureCell::~ProcedureCell()
{

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

CellPtr ProcedureCell::get_car() const
{
	throw runtime_error("try to access the car member of a non-cons Cell");
}

CellPtr ProcedureCell::get_cdr() const
{
	throw runtime_error("try to access the cdr member of a non-cons Cell");
}

CellPtr ProcedureCell::get_formals() const
{
	return formals_m;
}

CellPtr ProcedureCell::get_body() const
{
	return body_m;
}

void ProcedureCell::print(ostream& os) const
{
	os << "#<procedure>";
}

CellPtr ProcedureCell::eval()
{
	return shared_from_this();
}

CellPtr ProcedureCell::apply(CellPtr const args)
{
	symbol_table.push_front(map<string, CellPtr>());
	map<string, CellPtr>& local_table = *(symbol_table.begin());
	try{
		if(formals_m->is_symbol()){
			stack<CellPtr> arg_stack;
			CellPtr curr_cons = args;
			while(!curr_cons->is_nil()){
				arg_stack.push(curr_cons->get_car()->eval());
				curr_cons = curr_cons->get_cdr();
			}
			
			CellPtr arguments = smart_nil;
			while(!arg_stack.empty()){
				arguments = make_shared<ConsCell>(arg_stack.top(), arguments);
				arg_stack.pop();
			}
			local_table.insert(map<string, CellPtr>::value_type(formals_m->get_symbol(), arguments));
		}
		else{
			CellPtr form_cons = formals_m;
			CellPtr arg_cons = args;
			while(!(form_cons->is_nil() || arg_cons->is_nil())){
				local_table.insert(map<string, CellPtr>::value_type(form_cons->get_car()->get_symbol(), 	arg_cons->get_car()->eval()));
				form_cons = form_cons->get_cdr();
				arg_cons = arg_cons->get_cdr();
			}
	
			if(!(form_cons->is_nil() && arg_cons->is_nil()))
				throw runtime_error("arguments number mismatch");
		}
	
		CellPtr body_cons = body_m;
		while(!body_cons->get_cdr()->is_nil()){
			body_cons->get_car()->eval();
			body_cons = body_cons->get_cdr();
		}
		CellPtr result = body_cons->get_car()->eval();
		symbol_table.pop_front();
		return result;
	}catch(...){
		symbol_table.pop_front();
		throw;
	}
}
