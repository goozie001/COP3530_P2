#include <math.h>
#include <array>
#include <iostream>
#include <queue>

#ifndef OAHM_H
#define OAHM_H

namespace COP3530 {
    template <typename K, typename V, typename HASH_FUNC, HASH_FUNC hash, bool double_hash, typename PROBE_FUNC,
                PROBE_FUNC probe, bool op_equals = true, typename EQ_FUNC = bool, EQ_FUNC eq_test = false>
    class OAHM {

    private:
        struct HashEle {
            K key;
            V value;
            bool invalid;

            HashEle() {
                invalid = true;
            }

            HashEle(K key, V value) {
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
        HashEle* hashArray;

        void initContainers() {
            for (int i = 0; i < m; ++i)
                hashArray[i] = *(new HashEle);
        }

    public:

        OAHM() {
            m = 1024;
            hashArray = new HashEle[m]();
            used = 0;
            initContainers();
        }

        OAHM(std::size_t mySize) {
            double i;
            double d = log2(mySize);
            int power = modf(d, &i) == 0.0 ? d : d + 1;
            m = pow(2, power);
            const std::size_t a = m;
            hashArray = new HashEle[m]();
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
//                if (!double_hash) {
                    for (int j = 1; j < m; ++j) {
                        int p = (i + probe(j)) % m;
                        if (hashArray[p].invalid) {
                            hashArray[p].set(key, value);
                            ++used;
                            return true;
                        }
                    }
//                } else {
//                    int k = probe(key, m);
//                    for (int j = 1; j < m; ++j) {
//                        int p = (i + k * j) % m;
//                        if (hashArray[p].invalid) {
//                            hashArray[p].set(key, value);
//                            ++used;
//                            return true;
//                        }
//                    }
//                }
                return false;
            }
        }

        bool remove(K key, V& value) {
            bool result = false;
            if (used == 0)
                return result;
            else {
                std::queue<HashEle> slashSlingingSlasher;
                int i = hash(key, m), j = 0;
                int k = hash(key, m);
//                if (double_hash) {
//                    int re = probe(key, m);
//                    while (!is_empty() && !hashArray[i].invalid) {
//                        if (hashArray[i].key == key) {
//                            value = hashArray[i].value;
//                            hashArray[i].invalid = true;
//                            result = true;
//                            --used;
//                        } else {
//                            slashSlingingSlasher.push(hashArray[i]);
//                            hashArray[i].invalid = true;
//                            --used;
//                        }
//                        i = (k + re * ++j) % m;
//                    }
                //} else {
                    while (!is_empty() && !hashArray[i].invalid) {
                        if (hashArray[i].key == key) {
                            value = hashArray[i].value;
                            hashArray[i].invalid = true;
                            result = true;
                            --used;
                        } else {
                            slashSlingingSlasher.push(hashArray[i]);
                            hashArray[i].invalid = true;
                            --used;
                        }
                        i = (k + probe(++j)) % m;
                    //}
                }
                while (!slashSlingingSlasher.empty()) {
                    HashEle hashEle = slashSlingingSlasher.front();
                    insert(hashEle.key, hashEle.value);
                    slashSlingingSlasher.pop();
                }
                return result;
            }
        }

        bool search(K key, V& value) {
            if (used == 0)
                return false;
            else {
                int i = hash(key, m);
                if (hashArray[i].key == key && !hashArray[i].invalid) {
                    value = hashArray[i].value;
                    return true;
                } else {
                    if (!double_hash) {
                        for (int j = 1; j < m; ++j) {
                            int p = (probe(j) + i) % m;
                            if (hashArray[p].key == key && !hashArray[p].invalid) {
                                value = hashArray[p].value;
                                return true;
                            }
                        }
//                    } else {
//                        int k = probe(key, m);
//                        for (int j = 1; j < m; ++j) {
//                            int p = (i + k * j) % m;
//                            if (hashArray[p].key == key && !hashArray[p].invalid) {
//                                value = hashArray[p].value;
//                                return true;
//                            }
//                        }
                    }
                }
                return false;
            }
        }

        void clear() {
            delete hashArray;
            hashArray = new HashEle[m]();
            used = 0;
            initContainers();
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
            out << "[";
            for (int i = 0; i < m; ++i) {
                if (!hashArray[i].invalid) {
                    out << hashArray[i].key;
                } else {
                    out << "-";
                }
                if (i != m - 1)
                    out << ",";
            }
            out << "]" << std::endl;
        }
    };
}

template <typename K, typename V, typename HASH_FUNC, HASH_FUNC hash, bool double_hash, typename PROBE_FUNC,
        PROBE_FUNC probe, bool op_equals = true, typename EQ_FUNC = bool, EQ_FUNC eq_test = false>
class OAHM<K, V, HASH_FUNC, hash, true, PROBE_FUNC, probe, true, EQ_FUNC, eq_test> {

private:
    struct HashEle {
        K key;
        V value;
        bool invalid;

        HashEle() {
            invalid = true;
        }

        HashEle(K key, V value) {
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
    HashEle* hashArray;

    void initContainers() {
        for (int i = 0; i < m; ++i)
            hashArray[i] = *(new HashEle);
    }

public:

    OAHM() {
        m = 1024;
        hashArray = new HashEle[m]();
        used = 0;
        initContainers();
    }

    OAHM(std::size_t mySize) {
        double i;
        double d = log2(mySize);
        int power = modf(d, &i) == 0.0 ? d : d + 1;
        m = pow(2, power);
        const std::size_t a = m;
        hashArray = new HashEle[m]();
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
//                if (!double_hash) {
            for (int j = 1; j < m; ++j) {
                int p = (i + probe(j)) % m;
                if (hashArray[p].invalid) {
                    hashArray[p].set(key, value);
                    ++used;
                    return true;
                }
            }
//                } else {
//                    int k = probe(key, m);
//                    for (int j = 1; j < m; ++j) {
//                        int p = (i + k * j) % m;
//                        if (hashArray[p].invalid) {
//                            hashArray[p].set(key, value);
//                            ++used;
//                            return true;
//                        }
//                    }
//                }
            return false;
        }
    }

    bool remove(K key, V& value) {
        bool result = false;
        if (used == 0)
            return result;
        else {
            std::queue<HashEle> slashSlingingSlasher;
            int i = hash(key, m), j = 0;
            int k = hash(key, m);
//                if (double_hash) {
//                    int re = probe(key, m);
//                    while (!is_empty() && !hashArray[i].invalid) {
//                        if (hashArray[i].key == key) {
//                            value = hashArray[i].value;
//                            hashArray[i].invalid = true;
//                            result = true;
//                            --used;
//                        } else {
//                            slashSlingingSlasher.push(hashArray[i]);
//                            hashArray[i].invalid = true;
//                            --used;
//                        }
//                        i = (k + re * ++j) % m;
//                    }
            //} else {
            while (!is_empty() && !hashArray[i].invalid) {
                if (hashArray[i].key == key) {
                    value = hashArray[i].value;
                    hashArray[i].invalid = true;
                    result = true;
                    --used;
                } else {
                    slashSlingingSlasher.push(hashArray[i]);
                    hashArray[i].invalid = true;
                    --used;
                }
                i = (k + probe(++j)) % m;
                //}
            }
            while (!slashSlingingSlasher.empty()) {
                HashEle hashEle = slashSlingingSlasher.front();
                insert(hashEle.key, hashEle.value);
                slashSlingingSlasher.pop();
            }
            return result;
        }
    }

    bool search(K key, V& value) {
        if (used == 0)
            return false;
        else {
            int i = hash(key, m);
            if (hashArray[i].key == key && !hashArray[i].invalid) {
                value = hashArray[i].value;
                return true;
            } else {
                if (!double_hash) {
                    for (int j = 1; j < m; ++j) {
                        int p = (probe(j) + i) % m;
                        if (hashArray[p].key == key && !hashArray[p].invalid) {
                            value = hashArray[p].value;
                            return true;
                        }
                    }
//                    } else {
//                        int k = probe(key, m);
//                        for (int j = 1; j < m; ++j) {
//                            int p = (i + k * j) % m;
//                            if (hashArray[p].key == key && !hashArray[p].invalid) {
//                                value = hashArray[p].value;
//                                return true;
//                            }
//                        }
                }
            }
            return false;
        }
    }

    void clear() {
        delete hashArray;
        hashArray = new HashEle[m]();
        used = 0;
        initContainers();
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
        out << "[";
        for (int i = 0; i < m; ++i) {
            if (!hashArray[i].invalid) {
                out << hashArray[i].key;
            } else {
                out << "-";
            }
            if (i != m - 1)
                out << ",";
        }
        out << "]" << std::endl;
    }
};



#endif