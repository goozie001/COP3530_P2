#include <math.h>
#include <array>

#ifndef OAHM_H
#define OAHM_H

namespace COP3530 {
    template <typedef K, typedef V> class OAHM {

    private:

        const static float A = (sqrt(5) - 1) / 2;
        unsigned int m;
        int used;
        std::array<hashEle>* hashArray;

        struct hashEle {
            K key;
            V value;
            bool isValid;

            hashEle() {
                isValid = false;
            }
        };

        int hash(K key) {
            return (int) floor((m * ((A * key) % 1)));
        }

    public:

        OAHM() {
            m = 1024;
            hashArray = ne
            for (int i = 0; i < m; ++i) {
                hashArray[i] = new hashEle();
            }
            used = -1;
        }

        bool insert(K key, V value) {
            if (used == m)
                return false;
            else {
                int i = hash(key);
                while (hashArray[i].isValid)
            }
        }
    };

}


#endif