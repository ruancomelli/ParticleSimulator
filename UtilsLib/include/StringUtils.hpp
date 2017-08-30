#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>

namespace psin {

using std::string;

int stringCompare(const string & left, const string & right, const bool caseSensitive=false);
void toUpper(string & s);
void toLower(string & s);

} // psin

#endif
