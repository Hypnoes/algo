// bitmap.cpp -- Bitmap class
#include <fstream>
#include <memory>
#include "bitmap.h"

void Bitmap::init(int n)
{
    M = new char[N = (n + 7) / 8];
    std::memset(M, 0, N);
}

Bitmap::Bitmap(const char * file, int n = 8)
{
    init(n);
    std::ifstream ifs;
    ifs.open(file, std::ios_base::in);
    ifs.read(M, N);
    ifs.close();
}

Bitmap::Bitmap(const std::string file, int n = 8)
{
    init(n);
    std::ifstream ifs;
    ifs.open(file, std::ios_base::in);
    ifs.read(M, N);
    ifs.close();
}

Bitmap::~Bitmap()
{
    delete [] M;
    M = 0;
}

void Bitmap::set(int k)
{
    expand(k);
    M[k >> 3] = (0x80 >> (k & 0x07));
}

void Bitmap::clear(int k)
{
    expand(k);
    M[k >> 3] &= ~(0x80 >> (k & 0x07));
}

bool Bitmap::test(int k)
{
    expand(k);
    return M[k >> 3] & (0x80 >> (k & 0x07));
}

void Bitmap::dump(const char * file)
{
    std::ofstream ofs;
    ofs.open(file, std::ios_base::out);
    ofs.write(M, N);
    ofs.close();
}

void Bitmap::dump(const std::string file)
{
    std::ofstream ofs;
    ofs.open(file, std::ios_base::out);
    ofs.write(M, N);
    ofs.close();
}

char * Bitmap::bits2string(int n)
{
    expand(n - 1);
    char * s = new char[n + 1];
    s[n] = '\0';
    for (int i = 0; i < n; i++)
        s[i] = test(i) ? '1' : '0';
    return s;
}

void Bitmap::expand(int k)
{
    if (k < 8 * N) return ;
    int oldN = N;
    char * oldM = M;
    init(2 * k);
    std::memcpy_s(M, N, oldM, oldN);
    delete [] oldM;
}

int primeNLT(int c, int n, const char * file)
{
    Bitmap B(file, n);
    while (c < n)
        if (B.test(c)) c++;
        else return c;
    return c;
}