// exception.cpp -- inply of exception class
#include "exception.h"

const std::ostream & operator<<(const std::ostream & os, Exception e)
{
    os << e._msg;
    return os;
}