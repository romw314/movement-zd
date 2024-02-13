#include <algorithm>
#include <cstdlib>
#include <utility>
#include <string>

#define to_chptr(x) (std::to_string(x).c_str())
#define to_chptr_as(x,y) ((std::to_string(x) + (y)).c_str())

using std::atoi;
using std::pair;
