#include <iostream>
#include <iomanip>
#include <string>
#include <cstdint>
#include "PipeReg.h"

/* dumpField
 * Outputs a string and a uint64_t using the indicated width and padding with 0s.
 * If newline is true, a newline is output afterward.
 *
 * @param: fieldname - string to output; width used is the size of the string
 * @param: width - width in which to output the uint64_t
 * @param: fieldvalue - uint64_t that is output in width columns and padded with 0s
 * @param: newline - if true a newline is output after the fieldname and field value
 */
void PipeReg::dumpField(std::string fieldname, int width, uint64_t fieldvalue, bool newline)
{
   std::cout << fieldname << std::hex << std::setw(width) << std::setfill('0') << fieldvalue;
   if (newline) std::cout << std::endl;
}   
