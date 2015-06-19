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


class IntCell:public Cell
{
public:
	IntCell(int i);
	virtual bool is_int() const;
	virtual int get_int() const;
	virtual void print(std::ostream& os = std::cout) const;
private:
	int int_m;
};


class DoubleCell:public Cell
{
public:
	DoubleCell(double d);
	virtual bool is_double() const;
	virtual double get_double() const;
	virtual void print(std::ostream& os = std::cout) const;

private:
	double double_m;
};


class SymbolCell:public Cell
{
public:
	SymbolCell(const std::string& s);
	virtual bool is_symbol() const;
	virtual bool is_nil() const;
	virtual std::string get_symbol() const;
	virtual void print(std::ostream& os = std::cout) const;
	virtual CellPtr eval();
private:
	std::string symbol_m;
};


class ConsCell:public Cell
{
public:
	ConsCell(CellPtr car, CellPtr cdr);
	virtual bool is_cons() const;
	virtual CellPtr get_car() const;
	virtual CellPtr get_cdr() const;
	virtual void print(std::ostream& os = std::cout) const;
	virtual CellPtr eval();

private:
	CellPtr car_m;
	CellPtr cdr_m;
};


class ProcedureCell:public Cell
{
public:
	ProcedureCell(CellPtr formals, CellPtr body);
	virtual bool is_procedure() const;
	virtual CellPtr get_formals() const;
	virtual CellPtr get_body() const;
	virtual void print(std::ostream& os = std::cout) const;
	virtual CellPtr apply(CellPtr const args);

private:
	CellPtr formals_m;
	CellPtr body_m;
};


class PrimitiveProcedureCell:public ProcedureCell
{
public:
	PrimitiveProcedureCell(CellPtr (*func)(CellPtr const));
	virtual CellPtr apply(CellPtr const args);
	virtual void print(std::ostream& os = std::cout) const;
	static CellPtr add(CellPtr const args);
	static CellPtr subtract(CellPtr const args);
	static CellPtr multiply(CellPtr const args);
	static CellPtr divide(CellPtr const args);
	static CellPtr less_than(CellPtr const args);
	static CellPtr ceiling(CellPtr const args);
	static CellPtr pri_floor(CellPtr const args);
	static CellPtr ifelse(CellPtr const args);
	static CellPtr quote(CellPtr const args);
	static CellPtr cons(CellPtr const args);
	static CellPtr car(CellPtr const args);
	static CellPtr cdr(CellPtr const args);
	static CellPtr nullp(CellPtr const args);
	static CellPtr intp(CellPtr const args);
	static CellPtr doublep(CellPtr const args);
	static CellPtr symbolp(CellPtr const args);
	static CellPtr listp(CellPtr const args);
	static CellPtr pri_not(CellPtr const args);
	static CellPtr define(CellPtr const args);
	static CellPtr pri_print(CellPtr const args);
	static CellPtr pri_eval(CellPtr const args);
	static CellPtr lambda(CellPtr const args);
	static CellPtr pri_apply(CellPtr const args);
	static CellPtr let(CellPtr const args);
	static CellPtr pri_set(CellPtr const args);
	static CellPtr pri_exit(CellPtr const args);
	static CellPtr loadfile(CellPtr const args);
	static std::map<std::string, CellPtr> create_map();

private:
	template <typename IntOp, typename DoubleOp>
	static CellPtr arithmetic_operation(CellPtr const operands, IntOp int_op, DoubleOp double_op, const std::string& op);
	CellPtr (*func_m)(CellPtr const);
};


extern Cell* const nil;
extern CellPtr const smart_nil;
extern std::list<std::map<std::string, CellPtr> > symbol_table;

std::map<std::string, CellPtr>::iterator search_table(const std::string& s);

#endif
