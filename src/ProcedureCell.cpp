#include "ProcedureCell.hpp"
#include "Scope.hpp"
#include "ConsCell.hpp"
#include <stack>
using namespace std;

ProcedureCell::ProcedureCell(CellPtr formals, CellPtr body, ScopePtr parent_scope)
:formals_m(formals), body_m(body), parent_scope_sp(parent_scope)
{

}

ProcedureCell::ProcedureCell(CellPtr formals, CellPtr body, ScopeWPtr parent_scope)
:formals_m(formals), body_m(body), parent_scope_wp(parent_scope)
{

}

bool ProcedureCell::is_procedure() const
{
	return true;
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

CellPtr ProcedureCell::apply(const CellPtr& args)
{
	is_sp_needed = false;
	ScopePtr local_scope = make_shared<Scope>(parent_scope_wp);
	if(parent_scope_sp)
		local_scope = make_shared<Scope>(ScopeWPtr(parent_scope_sp));
	if(formals_m->is_symbol()){
		stack<CellPtr> arg_stack;
		CellPtr curr_cons = args;
		while(!curr_cons->is_nil()){
			arg_stack.push(curr_cons->get_car());
			curr_cons = curr_cons->get_cdr();
		}
		
		CellPtr arguments = smart_nil;
		while(!arg_stack.empty()){
			arguments = make_shared<ConsCell>(arg_stack.top(), arguments);
			arg_stack.pop();
		}
		local_scope->insert(Scope::value_type(formals_m->get_symbol(), arguments));
	}
	else{
		CellPtr form_cons = formals_m;
		CellPtr arg_cons = args;
		while(!(form_cons->is_nil() || arg_cons->is_nil())){
			local_scope->insert(Scope::value_type(form_cons->get_car()->get_symbol(), arg_cons->get_car()));
			form_cons = form_cons->get_cdr();
			arg_cons = arg_cons->get_cdr();
		}

		if(!(form_cons->is_nil() && arg_cons->is_nil()))
			throw runtime_error("arguments number mismatch");
	}

	ScopePtr temp = current_scope;
	current_scope = local_scope;
	try{
		CellPtr body_cons = body_m;
		while(!body_cons->get_cdr()->is_nil()){
			body_cons->get_car()->eval();
			body_cons = body_cons->get_cdr();
		}
		is_sp_needed = true;
		CellPtr result = body_cons->get_car()->eval();
		current_scope = temp;
		is_sp_needed = false;
		return result;
	}catch(...){
		current_scope = temp;
		is_sp_needed = false;
		throw;
	}
}

PrimitiveProcedureCell::PrimitiveProcedureCell(CellPtr (*func)(const CellPtr&))
	:ProcedureCell(smart_nil, smart_nil, ScopeWPtr(global_scope)), func_m(func)
{

}

CellPtr PrimitiveProcedureCell::apply(const CellPtr& args)
{
	return func_m(args);
}

void PrimitiveProcedureCell::print(ostream& os) const
{
	os << "#<primitive>";
}
