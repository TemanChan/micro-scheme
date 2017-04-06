#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <iostream>
#include <sstream>
#include <string>

#if __cplusplus > 201402L

#include <experimental/string_view>
typedef std::experimental::string_view string_type;
int stoi(const std::experimental::string_view &s);
double stod(const std::experimental::string_view &s);

#else
typedef std::string string_type;
#endif

class Tokenizer
{
public:
    Tokenizer();
    ~Tokenizer();
    void feed(const std::string &s);
    bool has_next();
    string_type next();

private:
    void skip_spaces();
    int pos;
    string_type *str_p;
};

#endif
