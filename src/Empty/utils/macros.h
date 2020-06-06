#pragma once

#include <iostream>
#include <sstream>

#define CALL_SITE __FILE__, __LINE__

/**
 * Prints data to std::cerr. Use the operator<< notation to print sequentially.
 */
#define TRACE(s) std::cerr << __FILE__ << ":" << __LINE__ << " : " << s << std::endl

/**
 * Raises a runtime_error exception with the given message. Use the operator<< notation to print sequentially.
 */
#define FATAL(s) do { std::stringstream ss; ss << __FILE__ << ":" << __LINE__ << " : " << s << std::endl; throw std::runtime_error(ss.str()); } while(0)

#ifdef _DEBUG
#define ASSERT(t) if(!(t)) FATAL(#t)
#else
#define ASSERT(t)
#endif
