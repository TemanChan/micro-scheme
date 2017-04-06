#include "Parser.hpp"
#include "Cell.hpp"
#include "IntCell.hpp"
#include "DoubleCell.hpp"
#include "SymbolCell.hpp"
#include "ConsCell.hpp"
#include <stack>
#include <stdexcept>

using namespace std;

Parser::Parser(){
    lp = make_shared<SymbolCell>("(");
}

void Parser::parse_eval_print(istream &is, ostream &os) const{
    string sexpr;
    Tokenizer t;
    stack<CellPtr> s;
    int num_lp = 0; // number of left parenthesis
    int i = 0;
    if(&is == &cin) os << "> ";
    do{
        while(getline(is, sexpr)){
            t.feed(sexpr);
            while(t.has_next()){
                const string &token = t.next();
                if(token[0] == ';'){
                    // support comments
                    t.feed("");
                    continue;
                }
                //cout << ++i << ", " << token.size() << ", " << token << endl;
                if(token == "("){
                    s.push(lp);
                    ++num_lp;
                }else if(token == ")"){
                    CellPtr cur = smart_nil;
                    while(!s.empty() && s.top() != lp){
                        cur = make_shared<ConsCell>(s.top(), cur);
                        s.pop();
                    }
                    if(s.empty())
                        throw runtime_error("unmatched parentheses");
                    s.pop();
                    s.push(cur);
                    --num_lp;
                }else
                    s.push(to_basic_cell(token));
            }
            if(num_lp == 0 && !s.empty()){
                if(s.size() != 1)
                    throw runtime_error("illegal s-expression");
                eval_print(s.top(), os);
                s.pop();
                if(&is == &cin) os << "> ";
            }
        }
    }while(num_lp > 0 || !is.eof());
    if(num_lp > 0)
        throw runtime_error("unmatched parentheses");

}

void Parser::eval_print(const CellPtr &root, ostream &os) const{
    try{
        const CellPtr &result = root->eval();
        if(result != smart_nil)
            os << *result << endl;
    }catch(runtime_error &e){
        cerr << "ERROR: " << e.what() << endl;
    }catch(logic_error &e){
        cerr << "LOGIC ERROR: " << e.what() << endl;
        exit(1);
    }
}

CellPtr Parser::to_basic_cell(const string &s) const{
    int type = 0; // 0 Int, 1 Double, 2 Symbol
    int i = 0;
    if(s[0] == '+' || s[0] == '-'){
        ++i;
        if(s.size() == 1) type = 2;
    }
    for(; i < s.size(); ++i){
        char c = s[i];
        if(isdigit(c)) continue;
        if(c == '.' && type == 0)
            type = 1;
        else{
            type = 2;
            break;
        }
    }

    if(type == 0) return make_shared<IntCell>(stoi(s));
    if(type == 1) return make_shared<DoubleCell>(stod(s));
    if(isdigit(s[0]))
        throw runtime_error("illegal starting character of identifier");
    return make_shared<SymbolCell>(s);
}
