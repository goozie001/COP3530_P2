#include <math.h>
#include <array>
#include <iostream>
#include <queue>

#ifndef BHM_H
#define BHM_H

namespace COP3530 {
    template <typename K, typename V, bool op_equals = true, typename EQ_FUNC = bool,
            EQ_FUNC eq_test = false>
    class BHM;

    template <typename K, typename V>
    class BHM<K, V> {

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

        int insert(K key, V value) {
            int probe = -1;
            int i = hash(key, m);
            try {
                HashEle *newEle = new HashEle(key, value);
            } catch (std::bad_alloc&) {
                return probe;
            }
            HashEle* ele = hashArray[i];
            if (ele == NULL) {
                hashArray[i] = newEle;
                ++used;
            }
            else {
                while (ele->next != NULL) {
                    ++probe;
                    ele = ele->next;
                }
                ele->next = newEle;
            }
            ++numOfEle;
            return probe;
        }

        int remove(K key, V& value) {
            int probe = -1;
            int i = hash(key, m);
            HashEle* ele = hashArray[i];
            HashEle* prev = NULL;
            if (used == 0 || ele == NULL)
                return -1  * m;
            while (ele != NULL && ele->key != key) {
                ++probe;
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
                --numOfEle;
            }
            return probe;
        }

        int search(K key, V& value) {
            int probe = 0;
            int i = hash(key, m);
            HashEle* ele = hashArray[i];
            while(ele != NULL) {
                if (ele->key == key) {
                    value = ele->value;
                    return probe;
                }
                ele = ele->next;
                ++probe;
            }
            return -1 * probe;
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

        BHM<int, int> cluster_distribution() {
            BHM<int, int> myMap(m / 2);
            for (int i = 0; i < m; ++i) {
                HashEle* ele = hashArray[i];
                int a = 0;
                while (ele != NULL) {
                    ++a;
                    ele = ele->next;
                }
                if (a != 0) {
                    int x;
                    if (myMap.search(a, x) > -1) {
                        myProbe.insert(a, ++x);
                    }
                    else {
                        myMap.insert(a, 1);
                    }
                }
            }
            return myMap;
        }

        K remove_random() {
            int i = rand() % size();
            int a = -1;
            int bucket = 0;
            HashEle* ele;
            for (int j = 0; j < m; ++j) {
                ele = hashArray[j];
                if (ele != NULL) {
                    ++a;
                    if (a == i)
                        break;
                }
                while (ele->next != NULL) {
                    ele = ele->next;
                    ++a;
                    if (a == i)
                        break;
                }
            }
            K key = ele->key;
            V val = ele->value;
            remove(key, val);
            return key;
        }
    };

    template <typename K, typename V, typename HASH_FUNC, HASH_FUNC hash, typename EQ_FUNC, EQ_FUNC eq_test>
    class BHM<K, V, HASH_FUNC, hash, false, EQ_FUNC, eq_test> {

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

        int insert(K key, V value) {
            int probe = -1;
            int i = hash(key, m);
            try {
                HashEle *newEle = new HashEle(key, value);
            } catch (std::bad_alloc&) {
                return probe;
            }
            HashEle* ele = hashArray[i];
            if (ele == NULL) {
                hashArray[i] = newEle;
                ++used;
            }
            else {
                while (ele->next != NULL) {
                    ++probe;
                    ele = ele->next;
                }
                ele->next = newEle;
            }
            ++numOfEle;
            return probe;
        }

        int remove(K key, V& value) {
            int probe = -1;
            int i = hash(key, m);
            HashEle* ele = hashArray[i];
            HashEle* prev = NULL;
            if (used == 0 || ele == NULL)
                return -1  * m;
            while (ele != NULL && ele->key != key) {
                ++probe;
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
                --numOfEle;
            }
            return probe;
        }

        int search(K key, V& value) {
            int probe = 0;
            int i = hash(key, m);
            HashEle* ele = hashArray[i];
            while(ele != NULL) {
                if (eq_test(ele->key, key)) {
                    value = ele->value;
                    return probe;
                }
                ele = ele->next;
                ++probe;
            }
            return -1 * probe;
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

        BHM<int, int> cluster_distribution() {
            BHM<int, int> myMap(m / 2);
            for (int i = 0; i < m; ++i) {
                HashEle* ele = hashArray[i];
                int a = 0;
                while (ele != NULL) {
                    ++a;
                    ele = ele->next;
                }
                if (a != 0) {
                    int x;
                    if (myMap.search(a, x) > -1) {
                        myProbe.insert(a, ++x);
                    }
                    else {
                        myMap.insert(a, 1);
                    }
                }
            }
            return myMap;
        }

        K remove_random() {
            int i = rand() % size();
            int a = -1;
            int bucket = 0;
            HashEle* ele;
            for (int j = 0; j < m; ++j) {
                ele = hashArray[j];
                if (ele != NULL) {
                    ++a;
                    if (a == i)
                        break;
                }
                while (ele->next != NULL) {
                    ele = ele->next;
                    ++a;
                    if (a == i)
                        break;
                }
            }
            K key = ele->key;
            V val = ele->value;
            remove(key, val);
            return key;
        }
    };
}


#endif