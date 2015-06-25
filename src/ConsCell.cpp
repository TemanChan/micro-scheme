#include <stack>
#include "ConsCell.hpp"
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
	CellPtr c = get_car()->eval();
	if(c->is_macro())
		return c->transform_eval(get_cdr());
	else if(c->is_procedure()){
		stack<CellPtr> arg_stack;
		CellPtr curr_cons = get_cdr();
		while(!curr_cons->is_nil()){
			arg_stack.push(curr_cons->get_car()->eval());
			curr_cons = curr_cons->get_cdr();
		}
		
		CellPtr arguments = smart_nil;
		while(!arg_stack.empty()){
			arguments = make_shared<ConsCell>(arg_stack.top(), arguments);
			arg_stack.pop();
		}
		return c->apply(arguments);
	}
	else
		throw runtime_error("expecting a procedure that can be applied to arguments");
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
