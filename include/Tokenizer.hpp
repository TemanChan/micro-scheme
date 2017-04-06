#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <iostream>
#include <sstream>
#include <string>

class Tokenizer
{
public:
    Tokenizer();
    ~Tokenizer();
    void feed(const std::string &s);
    bool has_next();
    std::string next();

private:
    std::stringstream *is_p;
    std::stringstream *ss_p;
};

#endif
