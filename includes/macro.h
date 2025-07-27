#pragma once

#include <cassert>
#include <cmath>

// Cross-platform debug halt
#ifdef _WIN32
    #include <intrin.h>
    #define DBG_HALT __debugbreak()
#elif defined(__linux__) || defined(__APPLE__)
    #include <csignal>
    #define DBG_HALT raise(SIGTRAP)
#else
    #define DBG_HALT std::abort()
#endif

// Cross-platform macro definitions for debugging and validation:
#ifdef DEBUG
    #define DBG_EXEC(exp) exp
    #define DBG_ASSERT(exp) { if(!(exp)){ DBG_HALT; } }
    #define DBG_VALID_FLOAT(f) { DBG_ASSERT(std::isfinite(f)); }
    #define DBG_VALID_VEC(v) { DBG_VALID_FLOAT(v.x); DBG_VALID_FLOAT(v.y); DBG_VALID_FLOAT(v.z); }
    #define DBG_VALID_QUAT(q) { DBG_VALID_FLOAT(q.w); DBG_VALID_FLOAT(q.x); DBG_VALID_FLOAT(q.y); DBG_VALID_FLOAT(q.z); }
    #define DBG_VALID_MAT(m) { for(int i = 0; i < m.nbrOfRow * m.nbrOfCol; i++) DBG_VALID_FLOAT(m.data[i]); }
#else
    #define DBG_EXEC(exp)
    #define DBG_ASSERT(exp)
    #define DBG_VALID_FLOAT(f)
    #define DBG_VALID_VEC(v)
    #define DBG_VALID_QUAT(q)
    #define DBG_VALID_MAT(m)
#endif

// Safe delete macros, already portable
#ifndef SAFE_DELETE
    #define SAFE_DELETE(exp) { if ((exp) != nullptr) { delete (exp); (exp) = nullptr; } }
#endif

#ifndef SAFE_DELETE_TAB
    #define SAFE_DELETE_TAB(exp) { if ((exp) != nullptr) { delete[] (exp); (exp) = nullptr; } }
#endif