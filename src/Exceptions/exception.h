// exception.h -- define exception class
#ifndef EXCEPTION_H_
#define EXCEPTION_H_
#include <iostream>

class Exception
{
  private:
    unsigned _errno;
    wchar_t * _msg;

  public:
    Exception() : _errno(0), _msg(L"\0") {}
    Exception(wchar_t * msg, unsigned errno = 0)
        : _errno(errno), _msg(msg) {}

    friend const std::wostream & operator<<(const std::wostream & os, 
                                           Exception e);
};

#endif