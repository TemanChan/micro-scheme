#include "Tokenizer.hpp"
#include <cctype> // isspace, isdigit
using namespace std;

Tokenizer::Tokenizer(){
    str_p = new string_type();
    pos = 0;
}

Tokenizer::~Tokenizer(){
    delete str_p;
}

void Tokenizer::skip_spaces(){
    const string_type &str = (*str_p);
    while(pos < str.size() && isspace(str[pos]))
        ++pos;
}

void Tokenizer::feed(const string &s){
    delete str_p;
    str_p = new string_type(s);
    pos = 0;
    skip_spaces();
}

bool Tokenizer::has_next(){
    return pos < str_p->size();
}

string_type Tokenizer::next(){
    const string_type &str = (*str_p);
    int last = pos;
    while(last < str.size()){
        char c = str[last];
        if(isspace(c) || c == '(' || c == ')')
            break;
        ++last;
    }
    if(last == pos){ // str[pos] is a parenthesis
        ++last;
    }
    const string_type &res = str.substr(pos, last - pos);
    pos = last;
    skip_spaces();
    return res;
}

#if __cplusplus > 201402L
using std::experimental::string_view;
int stoi(const string_view &s){
    return stoi(s.to_string());
}

double stod(const string_view &s){
    return stod(s.to_string());
}
#endif


