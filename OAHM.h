#include <math.h>
#include <array>
#include <iostream>
#include <queue>

#ifndef OAHM_H
#define OAHM_H

inline int intHash(int key, std::size_t m) {
    const double A = (sqrt(5) - 1) / 2;
    if (key < 0)
        key = key * -1;
    double intPart;
    return (int)(m * modf(A * key, &intPart));
}

inline int lin_probe(int i) { return i; }

namespace COP3530 {
    template<typename K, typename V, typename HASH_FUNC, HASH_FUNC hash, bool double_hash, typename PROBE_FUNC,
            PROBE_FUNC probe, bool op_equals = true, typename EQ_FUNC = bool, EQ_FUNC eq_test = false>
    class OAHM;

    template<typename K, typename V, typename HASH_FUNC, HASH_FUNC hash, typename PROBE_FUNC,
            PROBE_FUNC probe>
    class OAHM<K, V, HASH_FUNC, hash, false, PROBE_FUNC, probe> {

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
        HashEle *hashArray;

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

        int insert(K key, V value) {
            int probes = 0;
            int i = hash(key, m);
            if (hashArray[i].invalid || hashArray[i].key == key) {
                hashArray[i].set(key, value);
                ++used;
                return probes;
            }
            for (int j = 1; j < m; ++j) {
                int p = (i + probe(j)) % m;
                if (hashArray[p].invalid || hashArray[i].key == key) {
                    ++probes;
                    hashArray[p].set(key, value);
                    ++used;
                    return probes;
                }
            }
            return -1 * probes;

        }

        int remove(K key, V &value) {
            bool looking = true;
            int probes = -1;
            if (used == 0)
                return -1 * m;
            else {
                std::queue<HashEle> slashSlingingSlasher;
                int i = hash(key, m), j = 0;
                int k = hash(key, m);
                while (!is_empty() && !hashArray[i].invalid) {
                    if (looking)
                        ++probes;
                    if (hashArray[i].key == key) {
                        value = hashArray[i].value;
                        hashArray[i].invalid = true;
                        looking = false;
                        --used;
                    } else {
                        slashSlingingSlasher.push(hashArray[i]);
                        hashArray[i].invalid = true;
                        --used;
                    }
                    i = (k + probe(++j)) % m;
                }
                while (!slashSlingingSlasher.empty()) {
                    HashEle hashEle = slashSlingingSlasher.front();
                    insert(hashEle.key, hashEle.value);
                    slashSlingingSlasher.pop();
                }
                return (looking) ? -1 * probes : probes;
            }
        }

        int search(K key, V &value) {
            int probes = 0;
            if (used == 0)
                return -1 * m;
            else {
                int i = hash(key, m);
                if (hashArray[i].key == key && !hashArray[i].invalid) {
                    value = hashArray[i].value;
                    return probes;
                } else {
                    for (int j = 1; j < m; ++j) {
                        ++probes;
                        int p = (probe(j) + i) % m;
                        if (hashArray[p].key == key && !hashArray[p].invalid) {
                            value = hashArray[p].value;
                            return probes;
                        }
                    }
                }
                return -1 * probes;
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

        void print(std::ostream &out) {
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

        COP3530::OAHM<int, int, int(*)(int, std::size_t), intHash,
                false, int(*)(int), lin_probe> cluster_distribution() {
            int clustSize = 0;
            int initial = 0;
            COP3530::OAHM<int, int, int(*)(int, std::size_t), intHash,
                    false, int(*)(int), lin_probe> myProbe(m / 2);
            bool clusta = false;
            bool first = true;
            for (int i = 0; i < m; ++i) {
                if (hashArray[i].invalid) {
                    if (first)
                        first = false;
                    if (clusta) {
                        int a;
                        if (myProbe.search(clustSize, a) > -1) {
                            myProbe.insert(clustSize, ++a);
                            clustSize = 0;
                        }
                        else {
                            myProbe.insert(clustSize, 1);
                            clustSize = 0;
                        }
                        clusta = false;
                    }
                } else {
                    if (first)
                        ++initial;
                    else
                        ++clustSize;
                    if (!clusta)
                        clusta = true;
                }
            }
            if (!hashArray[m - 1].invalid) {
                initial += clustSize;
                int a;
                if (myProbe.search(initial, a) > -1) {
                    myProbe.insert(initial, ++a);
                    clustSize = 0;
                }
                else {
                    myProbe.insert(initial, 1);
                    clustSize = 0;
                }
            }
            return myProbe;
        }

        void print_all(std::ostream &out) {
            out << "[";
            for (int i = 0; i < m; ++i) {
                if (hashArray[i].invalid == 0) {
                    out << "(" << hashArray[i].key << "," << hashArray[i].value << ")";
                } else {
                    out << "-";
                }
                if (i != m - 1)
                    out << ",";
            }
            out << "]" << std::endl;
        }
    };

    template<typename K, typename V, typename HASH_FUNC, HASH_FUNC hash, typename PROBE_FUNC,
            PROBE_FUNC probe>
    class OAHM<K, V, HASH_FUNC, hash, true, PROBE_FUNC, probe> {

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
        HashEle *hashArray;

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

        int insert(K key, V value) {
            int probes = 0;
            if (used == m)
                return -1*m;
            else {
                int i = hash(key, m);
                if (hashArray[i].invalid || hashArray[i].key == key) {
                    hashArray[i].set(key, value);
                    ++used;
                    return 0;
                }
                int k = probe(key, m);
                for (int j = 1; j < m; ++j) {
                    int p = (i + k * j) % m;
                    ++probes;
                    if (hashArray[p].invalid || hashArray[i].key == key) {
                        hashArray[p].set(key, value);
                        ++used;
                        return probes;
                    }
                }
                return -1 * probes;
            }
        }

        int remove(K key, V &value) {
            bool looking = true;
            int probes = -1;
            if (used == 0)
                return -1 * m;
            else {
                std::queue<HashEle> slashSlingingSlasher;
                int i = hash(key, m), j = 0;
                int k = hash(key, m);
                int re = probe(key, m);
                while (!is_empty() && !hashArray[i].invalid) {
                    if (looking)
                        ++probes;
                    if (hashArray[i].key == key) {
                        value = hashArray[i].value;
                        hashArray[i].invalid = true;
                        looking = false;
                        --used;
                    } else {
                        slashSlingingSlasher.push(hashArray[i]);
                        hashArray[i].invalid = true;
                        --used;
                    }
                    i = (k + re * ++j) % m;
                }
                while (!slashSlingingSlasher.empty()) {
                    HashEle hashEle = slashSlingingSlasher.front();
                    insert(hashEle.key, hashEle.value);
                    slashSlingingSlasher.pop();
                }
                return looking ? -1 * probes : probes;
            }
        }

        int search(K key, V &value) {
            int probes = 0;
            if (used == 0)
                return -1 * m;
            else {
                int i = hash(key, m);
                if (hashArray[i].key == key && !hashArray[i].invalid) {
                    value = hashArray[i].value;
                    return probes;
                } else {
                    int k = probe(key, m);
                    for (int j = 1; j < m; ++j) {
                        ++probes;
                        int p = (i + k * j) % m;
                        if (hashArray[p].key == key && !hashArray[p].invalid) {
                            value = hashArray[p].value;
                            return probes;
                        }
                    }
                }
                return -1 * probes;
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

        void print(std::ostream &out) {
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

        COP3530::OAHM<int, int, int(*)(int, std::size_t), intHash,
                false, int(*)(int), lin_probe> cluster_distribution() {
            int clustSize = 0;
            COP3530::OAHM<int, int, int(*)(int, std::size_t), intHash,
                    false, int(*)(int), lin_probe> myProbe(m / 2);
            bool clusta = false;
            for (int i = 0; i < m; ++i) {
                if (hashArray[i].invalid) {
                    if (clusta) {
                        int a;
                        if (myProbe.search(clustSize, a) > -1) {
                            myProbe.insert(clustSize, ++a);
                            clustSize = 0;
                        }
                        else {
                            myProbe.insert(clustSize, 1);
                            clustSize = 0;
                        }
                        clusta = false;
                    }
                } else {
                    ++clustSize;
                    if (!clusta)
                        clusta = true;
                }
            }
            return myProbe;
        }

        void print_all(std::ostream &out) {
            out << "[";
            for (int i = 0; i < m; ++i) {
                if (!hashArray[i].invalid) {
                    out << "(" << hashArray[i].key << "," << hashArray[i].value << ")";
                } else {
                    out << "-";
                }
                if (i != m - 1)
                    out << ",";
            }
            out << "]" << std::endl;
        }
    };

    template<typename K, typename V, typename HASH_FUNC, HASH_FUNC hash, typename PROBE_FUNC,
            PROBE_FUNC probe, typename EQ_FUNC, EQ_FUNC eq_test>
    class OAHM<K, V, HASH_FUNC, hash, false, PROBE_FUNC, probe, false, EQ_FUNC, eq_test> {

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
        HashEle *hashArray;

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

        int insert(K key, V value) {
            int probes = 0;
            if (used == m)
                return -1 * m;
            else {
                int i = hash(key, m);
                if (hashArray[i].invalid || hashArray[i].key == key) {
                    hashArray[i].set(key, value);
                    ++used;
                    return probes;
                }
                for (int j = 1; j < m; ++j) {
                    ++probes;
                    int p = (i + probe(j)) % m;
                    if (hashArray[p].invalid || hashArray[i].key == key) {
                        hashArray[p].set(key, value);
                        ++used;
                        return probes;
                    }
                }
                return -1 * probes;
            }
        }

        int remove(K key, V &value) {
            bool looking = true;
            int probes = -1;
            if (used == 0)
                return -1 * m;
            else {
                std::queue<HashEle> slashSlingingSlasher;
                int i = hash(key, m), j = 0;
                int k = hash(key, m);
                while (!is_empty() && !hashArray[i].invalid) {
                    if (looking)
                        ++probes;
                    if (eq_test(hashArray[i].key, key)) {
                        value = hashArray[i].value;
                        hashArray[i].invalid = true;
                        looking = false;
                        --used;
                    } else {
                        slashSlingingSlasher.push(hashArray[i]);
                        hashArray[i].invalid = true;
                        --used;
                    }
                    i = (k + probe(++j)) % m;
                }
                while (!slashSlingingSlasher.empty()) {
                    HashEle hashEle = slashSlingingSlasher.front();
                    insert(hashEle.key, hashEle.value);
                    slashSlingingSlasher.pop();
                }
                return looking ? -1 * probes : probes;
            }
        }

        int search(K key, V &value) {
            int probes = 0;
            if (used == 0)
                return -1 * m;
            else {
                int i = hash(key, m);
                if (eq_test(hashArray[i].key, key) && !hashArray[i].invalid) {
                    value = hashArray[i].value;
                    return probes;
                } else {
                    for (int j = 1; j < m; ++j) {
                        ++probes;
                        int p = (probe(j) + i) % m;
                        if (hashArray[p].key == key && !hashArray[p].invalid) {
                            value = hashArray[p].value;
                            return probes;
                        }
                    }
                }
                return -1 * probes;
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

        void print(std::ostream &out) {
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

        COP3530::OAHM<int, int, int(*)(int, std::size_t), intHash,
                false, int(*)(int), lin_probe> cluster_distribution() {
            int clustSize = 0;
            COP3530::OAHM<int, int, int(*)(int, std::size_t), intHash,
                    false, int(*)(int), lin_probe> myProbe(m / 2);
            bool clusta = false;
            for (int i = 0; i < m; ++i) {
                if (hashArray[i].invalid) {
                    if (clusta) {
                        int a;
                        if (myProbe.search(clustSize, a) > -1) {
                            myProbe.insert(clustSize, ++a);
                            clustSize = 0;
                        }
                        else {
                            myProbe.insert(clustSize, 1);
                            clustSize = 0;
                        }
                        clusta = false;
                    }
                } else {
                    ++clustSize;
                    if (!clusta)
                        clusta = true;
                }
            }
            return myProbe;
        }

        void print_all(std::ostream &out) {
            out << "[";
            for (int i = 0; i < m; ++i) {
                if (!hashArray[i].invalid) {
                    out << "(" << hashArray[i].key << "," << hashArray[i].value << ")";
                } else {
                    out << "-";
                }
                if (i != m - 1)
                    out << ",";
            }
            out << "]" << std::endl;
        }
    };

    template<typename K, typename V, typename HASH_FUNC, HASH_FUNC hash, typename PROBE_FUNC,
            PROBE_FUNC probe, typename EQ_FUNC, EQ_FUNC eq_test>
    class OAHM<K, V, HASH_FUNC, hash, true, PROBE_FUNC, probe, false, EQ_FUNC, eq_test> {

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
        HashEle *hashArray;

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

        int insert(K key, V value) {
            int probes = 0;
            if (used == m)
                return -1 * m;
            else {
                int i = hash(key, m);
                if (hashArray[i].invalid || hashArray[i].key == key) {
                    hashArray[i].set(key, value);
                    ++used;
                    return probes;
                }
                int k = probe(key, m);
                for (int j = 1; j < m; ++j) {
                    ++probes;
                    int p = (i + k * j) % m;
                    if (hashArray[p].invalid || hashArray[i].key == key) {
                        hashArray[p].set(key, value);
                        ++used;
                        return probes;
                    }
                }
                return -1 * probes;
            }
        }

        int remove(K key, V &value) {
            bool looking = true;
            int probes = -1;
            if (used == 0)
                return -1 * m;
            else {
                std::queue<HashEle> slashSlingingSlasher;
                int i = hash(key, m), j = 0;
                int k = hash(key, m);
                int re = probe(key, m);
                while (!is_empty() && !hashArray[i].invalid) {
                    if (looking)
                        ++probe;
                    if (eq_test(hashArray[i].key, key)) {
                        value = hashArray[i].value;
                        hashArray[i].invalid = true;
                        looking = false;
                        --used;
                    } else {
                        slashSlingingSlasher.push(hashArray[i]);
                        hashArray[i].invalid = true;
                        --used;
                    }
                    i = (k + re * ++j) % m;
                }
                while (!slashSlingingSlasher.empty()) {
                    HashEle hashEle = slashSlingingSlasher.front();
                    insert(hashEle.key, hashEle.value);
                    slashSlingingSlasher.pop();
                }
                return looking ? -1 * probes : probes;
            }
        }

        int search(K key, V &value) {
            int probes = 0;
            if (used == 0)
                return -1 * m;
            else {
                int i = hash(key, m);
                if (eq_test(hashArray[i].key, key) && !hashArray[i].invalid) {
                    value = hashArray[i].value;
                    return probes;
                } else {
                    int k = probe(key, m);
                    for (int j = 1; j < m; ++j) {
                        ++probes;
                        int p = (i + k * j) % m;
                        if (eq_test(hashArray[p].key, key) && !hashArray[p].invalid) {
                            value = hashArray[p].value;
                            return probes;
                        }
                    }
                }
                return -1 * probes;
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

        void print(std::ostream &out) {
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

        COP3530::OAHM<int, int, int(*)(int, std::size_t), intHash,
                false, int(*)(int), lin_probe> cluster_distribution() {
            int clustSize = 0;
            COP3530::OAHM<int, int, int(*)(int, std::size_t), intHash,
                    false, int(*)(int), lin_probe> myProbe(m / 2);
            bool clusta = false;
            for (int i = 0; i < m; ++i) {
                if (hashArray[i].invalid) {
                    if (clusta) {
                        int a;
                        if (myProbe.search(clustSize, a) > -1) {
                            myProbe.insert(clustSize, ++a);
                            clustSize = 0;
                        }
                        else {
                            myProbe.insert(clustSize, 1);
                            clustSize = 0;
                        }
                        clusta = false;
                    }
                } else {
                    ++clustSize;
                    if (!clusta)
                        clusta = true;
                }
            }
            return myProbe;
        }

        void print_all(std::ostream &out) {
            out << "[";
            for (int i = 0; i < m; ++i) {
                if (!hashArray[i].invalid) {
                    out << "(" << hashArray[i].key << "," << hashArray[i].value << ")";
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

#endif