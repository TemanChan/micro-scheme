/**
 * \file main.cpp
 *
 * Driver code implementing the main read-parse-eval-print loop.
 * Supports both (1) an interactive mode, and (2) a batch mode where
 * input expressions are read from the file specified by the first
 * command-line argument.
 */

#include <stdexcept>
#include "Parser.hpp"
#include <istream>
#include <sstream>
#include <fstream>
#include <limits> // numeric_limits

using namespace std;

const Parser parser;

/**
 * \brief Read, parse, evaluate, and print the expression one by one from
 * the standard input, interactively.
 */
void readconsole()
{
	parser.parse_eval_print(cin, cout);
}

/**
 * \brief Read, parse, evaluate the expression one by one from a file
 */
void readfile(const char *fn)
{
	ifstream ifs(fn);
	if(ifs.is_open()){
		parser.parse_eval_print(ifs, cout);
		ifs.close();
	}else
		throw runtime_error("cannot open file \"" + string(fn) + "\"");
}

/**
 * \brief Call either the batch or interactive main drivers.
 */
int main(int argc, char* argv[])
{
	readfile("library.scm");
	switch(argc) {
	case 1:
		// read from the standard input
		readconsole();
		exit(0);
		break;
	case 2:
		// read from a file
		readfile(argv[1]);
		break;
	default:
		cout << "too many arguments!" << endl;
		exit(0);
	}
	return 0;
}
