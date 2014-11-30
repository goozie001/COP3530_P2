#include <math.h>
#include <array>
#include <iostream>
#include <queue>

#ifndef BHM_H
#define BHM_H

namespace COP3530 {
    template <typename K, typename V, typename HASH_FUNC, HASH_FUNC hash, bool op_equals = true, typename EQ_FUNC = bool,
            EQ_FUNC eq_test = false>
    class BHM {

    private:
        struct HashEle {
            K key;
            V value;
            HashEle* next = NULL;

            HashEle() {
                next = NULL;
            }

            HashEle(K key, V value) {
                this->key = key;
                this->value = value;
                next = NULL;
            }

            void set(K key, V value) {
                this->key = key;
                this->value = value;
                next = NULL;
            }

            ~HashEle() {
                if (next != NULL) {
                    delete next;
                    next = NULL;
                }
            }
        };

        std::size_t m;
        std::size_t numOfEle;
        std::size_t used;
        HashEle** hashArray;

    public:

        BHM() {
            m = 1024;
            hashArray = new HashEle*[m];
            used = 0;
            numOfEle = 0;
            for (int i = 0; i < m; ++i)
                hashArray[i] = NULL;
        }

        BHM(std::size_t mySize) {
            double i;
            double d = log2(mySize);
            int power = modf(d, &i) == 0.0 ? d : d + 1;
            m = pow(2, power);
            const std::size_t a = m;
            hashArray = new HashEle*[m]();
            used = 0;
            numOfEle = 0;
            for (int i = 0; i < m; ++i)
                hashArray[i] = NULL;
        }

        ~BHM() {
            clear();
        }

        bool insert(K key, V value) {
            int i = hash(key, m);
            HashEle* newEle = new HashEle(key, value);
            HashEle* ele = hashArray[i];
            if (ele == NULL) {
                hashArray[i] = newEle;
                ++used;
            }
            else {
                while (ele->next != NULL) {
                    ele = ele->next;
                }
                ele->next = newEle;
            }
            ++numOfEle;
            return true;
        }

        bool remove(K key, V& value) {
            bool result = false;
            int i = hash(key, m);
            HashEle* ele = hashArray[i];
            HashEle* prev = NULL;
            if (used == 0 || ele == NULL)
                return result;
            while (ele != NULL && ele->key != key) {
                prev = ele;
                ele = ele->next;
            }
            if (ele != NULL) {
                if (ele == hashArray[i]) {
                    value = ele->value;
                    hashArray[i] = ele->next;
                    if (hashArray[i] == NULL)
                        --used;
                    ele->next = NULL;

                }
                else {
                    --used;
                    value = ele->value;
                    prev->next = ele->next;
                    ele->next = NULL;
                }
                delete ele;
                ele = NULL;
                result = true;
                --numOfEle;
            }
            return result;
        }

        bool search(K key, V& value) {
            bool result = false;
            int i = hash(key, m);
            HashEle* ele = hashArray[i];
            while(ele != NULL) {
                if (ele->key == key) {
                    value = ele->value;
                    result = true;
                    return result;
                }
                ele = ele->next;
            }
            return result;
        }

        void clear() {
            for (int i = 0; i < m; ++i) {
                if (hashArray[i] != NULL) {
                    delete hashArray[i];
                    hashArray[i] = NULL;
                }
            }
            used = 0;
            numOfEle = 0;
        }

        bool is_empty() {
            return numOfEle == 0;
        }

        std::size_t capacity() {
            return m;
        }

        std::size_t size() {
            return numOfEle;
        }

        double load() {
            return (double) used / (double) m;
        }

        void print(std::ostream& out) {
            out << "[";
            for (int i = 0; i < m; ++i) {
                HashEle* ele = hashArray[i];
                if (ele == NULL)
                    out << "-";
                else {
                    while (ele != NULL) {
                        out << ele->key;
                        if (ele->next != NULL)
                            out << ",";
                        ele = ele->next;
                    }
                }
                if (i != m - 1)
                    out << ",";
            }
            out << "]" << std::endl;
        }
    };
}


#endif