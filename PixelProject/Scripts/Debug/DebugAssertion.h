#pragma once

#include <iostream> // For std::cerr
#include <cstdlib>   // For std::abort

// TODO : (James) Add a flag so these can be included in release?

#ifndef NDEBUG // NDEBUG is defined by default in release builds
// Simple assert macro that prints the expression that failed, the file and line it failed on, and aborts the program
#define DEBUG_ASSERT(expression) \
if (!(expression)) { \
std::cerr << "Assertion failed: " << #expression \
<< " in " << __FILE__ << " line " << __LINE__ << std::endl; \
std::abort(); \
}
#else
#define DEBUG_ASSERT(expression) ((void)0);
#endif

#ifndef NDEBUG
#define DEBUG_ASSERT_MSG(expression, message) \
if (!(expression)) { \
std::cerr << "Assertion failed: " << #expression \
<< " in " << __FILE__ << " line " << __LINE__ << std::endl; \
std::cerr << "Message: " << message << std::endl; \
std::abort(); \
}
#else
#define DEBUG_ASSERT_MSG(expression, message) ((void)0);
#endif

// Same as above but can pass in T which will print out the value of the object and name of the object
#ifndef NDEBUG
#define DEBUG_ASSERT_OBJ(expression, object) \
if (!(expression)) { \
std::cerr << "Assertion failed: " << #expression \
<< " in " << __FILE__ << " line " << __LINE__ << std::endl; \
std::cerr << "Object: " << #object << " Value: " << object << std::endl; \
std::abort(); \
}
#else
#define DEBUG_ASSERT_OBJ(expression, object) ((void)0);
#endif