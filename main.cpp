#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "OAHM.h"

inline int hash(int key, std::size_t m) {
    const double A = (sqrt(5) - 1) / 2;
    double intPart;
    return (int)(m * modf(A * key, &intPart));
}

inline int linear_probe(int i) { return i; }
inline int quad_probe(int i) { return i*i; }
inline int rehash(int key, std::size_t m) { return key % (int)(m - 1) + 1; }

TEST_CASE("Open Address Hash Map: Require all basic methods to work as they are supposed to for linear probing", "[Linear Probe]") {
    COP3530::OAHM<int, char, int (*)(int, std::size_t), hash,
            false, int (*)(int), linear_probe> linearHash(65);

    SECTION("Test to see that, up to the capacity of the array, the hashing and probing will allow every slot to be used") {
        char s = 'A';
        // Capacity of the array is always a power of 2. 128 is the power of 2 greater than but closest to the size 65 specified.
        for (int i = -64; i < 64; ++i, ++s) {
            REQUIRE(linearHash.insert(i, s));
        }

            SECTION("Test to see that the remove function works properly.") {
                s = 'A';
                char c;
                for (int i = -64; i < 64; ++i, ++s) {
                    if (i == 0)
                        int a = 0;
                    REQUIRE(linearHash.remove(i, c));
                    REQUIRE(c == s);
                }
                REQUIRE(!linearHash.remove(2, c));
            }

        SECTION("Test to see if the search function works properly.") {
            char s = 'A';
            char c;
            for (int i = -64; i < 64; ++i, ++s) {
                REQUIRE(linearHash.search(i, c));
                REQUIRE(c == s);
            }
            REQUIRE(!linearHash.search(128, c));
        }
        SECTION("Require the print method to work properly") {
            REQUIRE_NOTHROW(linearHash.print(std::cout));
        }
        SECTION("Make sure size, capacity, and load all return correct values.") {
            // The size should equal the capacity since it is full.
            REQUIRE(linearHash.size() == 128);
            REQUIRE(linearHash.capacity() == linearHash.size());
            REQUIRE(linearHash.load() == 1);
        }
        SECTION("Test to see that clear() works in that it clear everything.") {
            REQUIRE_NOTHROW(linearHash.clear());
            REQUIRE(linearHash.is_empty());
        }
    }
}

TEST_CASE("Open Address Hash Map: Require all basic methods to work as they are supposed to for quadratic probing", "[Quadratic Probe]") {
    COP3530::OAHM<int, char, int (*)(int, std::size_t), hash,
            false, int (*)(int), quad_probe> quadHashMap(65);

    SECTION("Test to see that, up to the capacity of the array, the hashing and probing will allow every slot to be used") {
        char s = 'A';
        // With quadratic probing, I can't reach evey element without my probing function being (i*(i+1))/2, which isn't what is specified.
        // Therefore, I search for half of the list.
        for (int i = 0; i < 64; ++i, ++s) {
            if (i == 118) {
                std::cout << std::endl << std::endl;
                quadHashMap.print(std::cout);
            }
            REQUIRE(quadHashMap.insert(i, s));
        }
        SECTION("Test to see that insert returns false when the array is completely full.") {


            SECTION("Test to see that the remove function works properly.") {
                s = 'A';
                char c;
                for (int i = 0; i < 64; ++i, ++s) {
                    REQUIRE(quadHashMap.remove(i, c));
                    REQUIRE(c == s);
                }
                REQUIRE(!quadHashMap.remove(2, c));
            }
        }
        SECTION("Test to see if the search function works properly.") {
            s = 'A';
            char c;
            for (int i = 0; i < 64; ++i, ++s) {
                REQUIRE(quadHashMap.search(i, c));
                REQUIRE(c == s);
            }
            REQUIRE(!quadHashMap.search(64, c));
        }
        SECTION("Require the print method to work properly") {
            REQUIRE_NOTHROW(quadHashMap.print(std::cout));
        }
        SECTION("Make sure size, capacity, and load all return correct values.") {
            // The size should equal the capacity since it is full.
            REQUIRE(quadHashMap.size() == 64);
            REQUIRE(quadHashMap.capacity() == quadHashMap.size() * 2);
            REQUIRE(quadHashMap.load() == 0.5);
        }
        SECTION("Test to see that clear() works in that it clear everything.") {
            REQUIRE_NOTHROW(quadHashMap.clear());
            REQUIRE(quadHashMap.is_empty());
        }
    }
}

//TEST_CASE("Open Address Hash Map: Require all basic methods to work as they are supposed to for rehashing", "[Rehash]") {
//    COP3530::OAHM<int, char, int (*)(int, std::size_t), hash,
//            true, int (*)(int, std::size_t), rehash> hashHashMap(65);
//
//    SECTION("Test to see that, up to the capacity of the array, the hashing and rehashing will allow every slot to be used") {
//        char s = 'A';
//        // With quadratic probing, I can't reach evey element without my probing function being (i*(i+1))/2, which isn't what is specified.
//        // Therefore, I search for half of the list.
//        for (int i = 0; i < 64; ++i, ++s) {
//            if (i == 118) {
//                std::cout << std::endl << std::endl;
//                hashHashMap.print(std::cout);
//            }
//            REQUIRE(hashHashMap.insert(i, s));
//        }
//        SECTION("Test to see that insert returns false when the array is completely full.") {
//
//
//            SECTION("Test to see that the remove function works properly.") {
//                s = 'A';
//                char c;
//                for (int i = 0; i < 64; ++i, ++s) {
//                    REQUIRE(hashHashMap.remove(i, c));
//                    REQUIRE(c == s);
//                }
//                REQUIRE(!hashHashMap.remove(2, c));
//            }
//        }
//        SECTION("Test to see if the search function works properly.") {
//            s = 'A';
//            char c;
//            for (int i = 0; i < 64; ++i, ++s) {
//                REQUIRE(hashHashMap.search(i, c));
//                REQUIRE(c == s);
//            }
//            REQUIRE(!hashHashMap.search(64, c));
//        }
//        SECTION("Require the print method to work properly") {
//            REQUIRE_NOTHROW(hashHashMap.print(std::cout));
//        }
//        SECTION("Make sure size, capacity, and load all return correct values.") {
//            // The size should equal the capacity since it is full.
//            REQUIRE(hashHashMap.size() == 64);
//            REQUIRE(hashHashMap.capacity() == hashHashMap.size() * 2);
//            REQUIRE(hashHashMap.load() == 0.5);
//        }
//        SECTION("Test to see that clear() works in that it clear everything.") {
//            REQUIRE_NOTHROW(hashHashMap.clear());
//            REQUIRE(hashHashMap.is_empty());
//        }
//    }
//}