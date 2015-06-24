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
#include <stdexcept>
#include <map>
#include <iterator>
#include <utility> // pair
#include <memory> // shared_ptr, enable_shared_from_this

class Cell;
typedef std::shared_ptr<Cell> CellPtr;

/**
 * @brief A abstract base class for cells
 */
class Cell:public std::enable_shared_from_this<Cell>
{
public:
	virtual ~Cell(){}
	/**
	 * @brief Check if this is an int cell.
	 * @return True iff this is an int cell.
	 */
	virtual bool is_int() const;
	virtual bool is_double() const;
	virtual bool is_symbol() const;
	virtual bool is_cons() const;
	virtual bool is_procedure() const;
	virtual bool is_nil() const;
	virtual int get_int() const;
	virtual double get_double() const;
	virtual std::string get_symbol() const;
	virtual CellPtr get_car() const;
	virtual CellPtr get_cdr() const;
	virtual CellPtr get_formals() const;
	virtual CellPtr get_body() const;
	virtual void print(std::ostream& os = std::cout) const = 0;
	virtual CellPtr eval();
	virtual CellPtr apply(CellPtr const args);
};

extern Cell* const nil;
extern CellPtr const smart_nil;

class Scope;
typedef std::shared_ptr<Scope> ScopePtr;
extern ScopePtr global_scope;
extern ScopePtr current_scope;

#endif
