#include "Tokenizer.hpp"
#include <cctype> // isspace, isdigit
using namespace std;


Tokenizer::Tokenizer(){
    is_p = new stringstream;
    ss_p = new stringstream;
}

Tokenizer::~Tokenizer(){
    delete is_p;
    delete ss_p;
}

void Tokenizer::feed(const string &s){
    delete is_p;
    is_p = new stringstream(s);
    char c;
    // skip leading spaces
    while(is_p->get(c) && isspace(c));
    if(!is_p->eof()) is_p->unget();
}

bool Tokenizer::has_next(){
    return !is_p->eof();
}

string Tokenizer::next(){
    char c;
    ss_p->str(""); // clear contents in stringstream
    while(is_p->get(c) && !isspace(c)){
        if(c == '(' || c == ')'){
            if(ss_p->rdbuf()->in_avail() == 0) // first char is a parenthesis
                (*ss_p) << c;
            else // a parenthesis after some non-white-space chars
                is_p->unget();
            break;
        }
        (*ss_p) << c;
    }

    // is_p->eof() || isspace(c) || c is a parenthesis
    if(!is_p->eof()){
        while(is_p->get(c) && isspace(c));
        // is_p->eof() || !isspace(c)
        if(!is_p->eof()) is_p->unget();
    }
    return ss_p->str();
}
