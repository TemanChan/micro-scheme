#include <iostream>
#include <cstdlib>
#include <string>
/**
 * @brief An inline function that prints error messages and terminates the program.
 * @param msg The error message to be printed
 */
inline void errmsg(const std::string& msg)
{
	std::cerr << "Error: " << msg << std::endl;
	exit(1);
}
