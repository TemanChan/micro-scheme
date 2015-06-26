#include "Scope.hpp"
using namespace std;

Scope::Scope(ScopeWPtr parent, const Scope::Map& local)
:parent_scope(parent), local_scope(local)
{

}

pair<Scope::iterator, bool> Scope::insert(const value_type& val)
{
	return local_scope.insert(val);
}

Scope::iterator Scope::find(const key_type& k)
{
	Scope::iterator it = local_scope.find(k);
	if(it != local_scope.end())
		return it;
	if(parent_scope.lock())
		return parent_scope.lock()->find(k);
	return local_scope.end();
}

Scope::iterator Scope::end()
{
	return global_scope->local_scope.end();
}

Scope::size_type Scope::count(const key_type& k) const
{
	return local_scope.count(k);
}

CellPtr Scope::eval(const string& symbol)
{
	Scope::iterator it = local_scope.find(symbol);
	if(it != local_scope.end())
		return it->second;
	else if(parent_scope.lock())
		return parent_scope.lock()->eval(symbol);
	else
		throw runtime_error("try to evaluate an un-defined symbol \"" + symbol + "\"");
}
