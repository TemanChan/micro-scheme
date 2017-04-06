#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include "Cell.hpp"
#include "Tokenizer.hpp"

class Parser
{
public:
    Parser();
    //CellPtr parse(const std::string &s);
    void parse_eval_print(std::istream &is, std::ostream &os) const;

private:
    void eval_print(const CellPtr &root, std::ostream &os) const;
    CellPtr to_basic_cell(const std::string &s) const;
    CellPtr lp; // left parenthesis
};

#endif
