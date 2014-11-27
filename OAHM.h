#include <math.h>
#include <array>
#include <iostream>

#ifndef OAHM_H
#define OAHM_H

namespace COP3530 {
    template <typename K, typename V, typename HASH_FUNC, HASH_FUNC hash, bool double_hash, typename PROBE_FUNC,
                PROBE_FUNC probe, bool op_equals = true, typename EQ_FUNC = bool, EQ_FUNC eq_test = false>
    class OAHM {

    private:
        struct hashEle {
            K key;
            V value;
            bool invalid;

            hashEle() {
                invalid = true;
            }

            hashEle(K key, V value) {
                this->key = key;
                this->value = value;
                invalid = false;
            }

            void set(K key, V value) {
                this->key = key;
                this->value = value;
                invalid = false;
            }
        };

        std::size_t m;
        std::size_t used;
        hashEle* hashArray;

        void initContainers() {
            for (int i = 0; i < m; ++i)
                hashArray[i] = *(new hashEle);
        }

    public:

        OAHM() {
            m = 1024;
            hashArray = new hashEle[m]();
            used = 0;
            initContainers();
        }

        OAHM(std::size_t mySize) {
            int power = log2(mySize) + 1;
            m = pow(2, power);
            const std::size_t a = m;
            hashArray = new hashEle[m]();
            used = 0;
            initContainers();
        }

        ~OAHM() {
            delete hashArray;
        }

        bool insert(K key, V value) {
            if (used == m)
                return false;
            else {
                int i = hash(key, m);
                if (hashArray[i].invalid) {
                    hashArray[i].set(key, value);
                    ++used;
                    return true;
                }
                for (int j = 1; j < m; ++j) {
                    int p = (i + probe(j)) % m;
                    if (hashArray[p].invalid) {
                        hashArray[p].set(key, value);
                        ++used;
                        return true;
                    }
                }
                return false;
            }
        }

        bool remove(K key, V& value) {
            if (used == 0)
                return false;
            else {
                int i = hash(key);
                if (hashArray[i].key == key && !hashArray[i].invalid) {
                    value = hashArray[i].value;
                    hashArray[i].invalid = true;
                    --used;
                    return true;
                } else {
                    for (int j = 1; j < m; ++j) {
                        int p = probe(key, j);
                        if (hashArray[p].key == key && !hashArray[i].invalid) {
                            value = hashArray[i].value;
                            hashArray[i].invalid = true;
                            --used;
                            return true;
                        }
                    }
                }
                return false;
            }
        }

        bool search(K key, V& value) {
            if (used == 0)
                return false;
            else {
                int i = hash(key);
                if (hashArray[i].key == key && !hashArray[i].invalid) {
                    value = hashArray[i].value;
                    return true;
                } else {
                    for (int j = 1; j < m; ++j) {
                        int p = probe(key, j);
                        if (hashArray[p] != NULL && !hashArray[i].invalid) {
                            value = hashArray[p].key;
                            return true;
                        }
                    }
                }
                return false;
            }
        }

        void clear() {
            delete hashArray;
            hashArray = new std::array<hashEle, m>;
            used = 0;
        }

        bool is_empty() {
            return used == 0;
        }

        std::size_t capacity() {
            return m;
        }

        std::size_t size() {
            return used;
        }

        double load() {
            return (double) used / (double) m;
        }

        void print(std::ostream& out) {
            for (int i = 0; i < m; ++i) {
                if (!hashArray[i].invalid) {
                    out << hashArray[i].key << std::endl;
                } else {
                    out << "-" << std::endl;
                }
            }
        }
    };
}


#endif