#include "OAHM.h"

inline int hash(int key, std::size_t m) {
    const double A = (sqrt(5) - 1) / 2;
    double intPart;
    return (int)(m * modf((A / 2.0) * key, &intPart));
}
inline int quad_probe(int i) { return (i*(i + 1) / 2); }

int main() {
    COP3530::OAHM<int, char, int(*)(int,std::size_t), hash,
            false, int(*)(int), quad_probe> myHash(33);

    myHash.insert(1, 'A');
    myHash.insert(2, 'B');
    myHash.insert(3, 'C');
    myHash.insert(4, 'D');
    myHash.insert(5, 'E');
    myHash.insert(6, 'F');
    myHash.insert(7, 'G');
    myHash.insert(8, 'H');

    myHash.print(std::cout);
}