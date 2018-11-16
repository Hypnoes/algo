// bitmap.h -- Bitmap class
#ifndef BITMAP_H_
#define BITMAP_H_
#include <string>

class Bitmap
{
  private:
    char * M;
    int N;

  protected:
    void init(int n);

  public:
    Bitmap(int n = 8) { init(n); }
    Bitmap(const char * file, int n = 8);
    Bitmap(const std::string file, int n = 8);
    ~Bitmap();

    void set(int);
    void clear(int);
    bool test(int);

    void dump(const char * file);
    void dump(const std::string file);
    char * bits2string(int);
    void expand(int);
};

int primeNLT(int, int, const char *);

#endif