// exception.cpp -- inply of exception class
#include "exception.h"

const std::wostream & operator<<(const std::wostream & os, Exception e)
{
    os << e._msg;
    return os;
}