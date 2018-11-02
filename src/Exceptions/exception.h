// exception.h -- define exception class
#ifndef EXCEPTION_H_
#define EXCEPTION_H_
#include <iostream>

class Exception
{
  private:
    unsigned _errno;
    char * _msg;

  public:
    Exception() : _errno(0), _msg("\0") {}
    Exception(char * msg, unsigned errno = 0)
        : _errno(errno), _msg(msg) {}

    friend const std::ostream & operator<<(const std::ostream & os, 
                                           Exception e);
};

#endif