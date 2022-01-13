#pragma once

#include <iostream>
#include <sstream>
#include <cstring>

#ifdef _WIN32
#define __FILESTEM__ (strrchr(__FILE__, '\\') + 1)
#else
#define __FILESTEM__ (strrchr(__FILE__, '/') + 1)
#endif

#define CALL_SITE __FILESTEM__, __LINE__

/**
 * Print data to std::cerr. Use the operator<< notation to print sequentially.
 */
#define TRACE(s) std::cerr << __FILESTEM__ << ":" << __LINE__ << " : " << s << std::endl

/**
 * Raise a runtime_error exception with the given message. Use the operator<< notation to print sequentially.
 */
#define FATAL(s) do { std::stringstream ss; ss << __FILESTEM__ << ":" << __LINE__ << " : " << s << std::endl; throw std::runtime_error(ss.str()); } while(0)

/**
 * Perform a test that kills the program if it fails, even outside of debug mode.
 */
#define ALWAYS_ASSERT(t) if(!(t)) FATAL(#t)

// Define a few macros that do different things based on the debug level
#ifdef _DEBUG

// Perform a test that kills the program if it fails
#define ASSERT(t) ALWAYS_ASSERT(t)
// Do something only in debug mode
#define DEBUG_ONLY(x) x

#else // !defined(_DEBUG)

#define ASSERT(t)
#define DEBUG_ONLY(x)

#endif // _DEBUG
