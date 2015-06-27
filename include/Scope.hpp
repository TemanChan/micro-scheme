#ifndef SCOPE_HPP
#define SCOPE_HPP

#include "Cell.hpp"

class Scope
{
public:
	typedef std::map<std::string, CellPtr> Map;
	typedef Map::value_type value_type;
	typedef Map::key_type key_type;
	typedef Map::size_type size_type;
	typedef Map::iterator iterator;
	explicit Scope(ScopePtr parent = ScopePtr(), const Map& local = Map());
	std::pair<iterator, bool> insert(const value_type& val);
	iterator find(const key_type& k);
	iterator end();
	size_type count(const key_type& k) const;
	CellPtr eval(const std::string& symbol);

private:
	Map local_scope;
	ScopePtr parent_scope;
};

#endif
