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
	virtual bool is_int() const = 0;
	virtual bool is_double() const = 0;
	virtual bool is_symbol() const = 0;
	virtual bool is_cons() const = 0;
	virtual bool is_procedure() const = 0;
	virtual bool is_nil() const = 0;
	virtual int get_int() const = 0;
	virtual double get_double() const = 0;
	virtual std::string get_symbol() const = 0;
	virtual CellPtr get_car() const = 0;
	virtual CellPtr get_cdr() const = 0;
	virtual CellPtr get_formals() const = 0;
	virtual CellPtr get_body() const = 0;
	virtual void print(std::ostream& os = std::cout) const = 0;
	virtual CellPtr eval() = 0;
	virtual CellPtr apply(CellPtr const args) = 0;
};

extern Cell* const nil;
extern CellPtr const smart_nil;
extern std::list<std::map<std::string, CellPtr> > symbol_table;

std::map<std::string, CellPtr>::iterator search_table(const std::string& s);

#endif
