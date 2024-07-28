#pragma once
/*
In computer programming, a precompiled header is a (C or C++) header file that is compiled into 
an intermediate form that is faster to process for the compiler. Usage of precompiled headers may 
significantly reduce compilation time, especially when applied to large header files, header files 
that include many other header files, or header files that are included in many translation units.

To reduce compilation times, some compilers allow header files to be compiled into a form that is 
faster for the compiler to process. This intermediate form is known as a precompiled header, and 
is commonly held in a file named with the extension .pch or similar, such as .gch under the GNU 
Compiler Collection.
*/
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#ifdef SE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

