/**
 * \mainpage COMP2012H Programming Assignment 2, Fall 2014
 *
 * \author **Put your name here**
 * \author **Your studentID**
 * \author **Your email**
 * \author **Your lab session**
 *
 * \date **Fill in the date**
 *
 * Instructor: <a href="http://www.cs.ust.hk/~dekai/">Dekai Wu</a>
 * Due: 2014.10.10 at 23:00 by CASS
 */

/**
 * \file Cell.hpp
 *
 * Encapsulates the abstract interface for a concrete class-based
 * implementation of cells for a cons list data structure.
 */


#ifndef CELL_HPP
#define CELL_HPP

#include <iostream>
#include <string>
#include <cstddef>
#include <stdexcept>
#include <map>
#include <list>
#include <iterator>
#include <utility> // pair

/**
 * @brief A abstract base class for cells
 */
class Cell
{
public:
	virtual ~Cell(){}
	/**
	 * @brief A virtual copy constructor
	 * @return A pointer points to the clone
	 */
	virtual Cell* clone() const = 0;
	/**
	 * @brief Check if this is an int cell.
	 * @return True iff this is an int cell.
	 */
	virtual bool is_int() const = 0;
	virtual bool is_double() const = 0;
	virtual bool is_symbol() const = 0;
	virtual bool is_cons() const = 0;
	virtual bool is_procedure() const = 0;
	virtual bool is_nil() const = 0;
	virtual int get_int() const = 0;
	virtual double get_double() const = 0;
	virtual std::string get_symbol() const = 0;
	virtual const Cell* get_car() const = 0;
	virtual const Cell* get_cdr() const = 0;
	virtual const Cell* get_formals() const = 0;
	virtual const Cell* get_body() const = 0;
	virtual void print(std::ostream& os = std::cout) const = 0;
	virtual Cell* eval() const = 0;
	virtual Cell* apply(const Cell* const args) const = 0;
};

extern Cell* const nil;
extern std::list<std::map<std::string, Cell*> > symbol_table;

inline std::map<std::string, Cell*>::iterator search_table(const std::string& s)
{
	std::list<std::map<std::string, Cell*> >::iterator i = symbol_table.begin();
	std::map<std::string, Cell*>::iterator j;
	for(; i != symbol_table.end(); ++i){
		j = i->find(s);
		if(j != i->end())
			return j;
	}
	return j;
}

inline void safe_delete(Cell*& c)
{
	if(c != nil){
		delete c;
		c = nil;
	}
}

/**
 * @brief Destory the cells in the symbol table and the nil cell before the program terminates
 */
inline void free_mem_before_exit()
{
	std::list<std::map<std::string, Cell*> >::iterator i = symbol_table.begin();
	std::map<std::string, Cell*>::iterator j;
	for(; i != symbol_table.end(); ++i)
		for(j = i->begin(); j != i->end(); ++j)
			delete j->second;
	delete nil;
}

#endif
