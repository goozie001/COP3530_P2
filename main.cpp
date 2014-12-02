#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "RBST.h"
#include <cstring>

inline bool compare(char* str, char* str1) {
    return strcmp(str, str1) == 0;
}

inline bool compareStr(std::string str, std::string str1) {
    return str.compare(str1) == 0;
}

TEST_CASE("RBST: Require all basic methods to work as they are supposed to for all data types") {
    COP3530::RBST<int, char> tree(10);

    SECTION("Test to see that, up to the capacity of the array, you can insert properly") {
        char s = 'A';
        double d = 0.5;
        char* strang = "hahahaha";
        char* twoStrang = "hehehehehe";
        std::string str1 = "This is first";
        std::string str2 = "This is second";
        for (int i = 0; i < 10; ++i, ++s, ++d) {
            REQUIRE(tree.insert(i, s) > -1);
        }
        REQUIRE(tree.insert(100, s) <= -1);
        tree.print(std::cout);

        // TODO: Create another section
        s = 'A';
        char c;
        for (int i = 0; i < 10; ++i, ++s, ++d) {
            REQUIRE(tree.search(i, c) > -1);
            REQUIRE(c == s);
        }
        s = 'A';
        for (int i = 0; i < 10; ++i, ++s) {
            REQUIRE(tree.remove(i, s) > -1);
        }
        // These are to test out passing in the custom equals method (which these methods use except insert)
//        REQUIRE(cstringHash.insert(twoStrang, s) >= 0);
//        REQUIRE(cstringHash.insert(strang, s) >= 0);
//        REQUIRE(stringHash.insert(str1, s) >= 0);
//        REQUIRE(stringHash.insert(str2, s) > -1);
//        REQUIRE(cstringHash.search(strang, s) > -1);
//        REQUIRE(stringHash.search(str1, s) > -1);
//        REQUIRE(cstringHash.remove(twoStrang, s) > -1);
//        REQUIRE(stringHash.remove(str2, s) > -1);
//
//            SECTION("Test to see that the remove function works properly.") {
//                s = 'A';
//                char c;
//                d = 0.5;
//                for (int i = -64; i < 0; ++i, ++s, ++d) {
//                    REQUIRE(linearHash.remove(i, c) > -1);
//                    REQUIRE(dub.remove(d, c) > -1);
//                    REQUIRE(c == s);
//                }
//                REQUIRE(linearHash.remove(2, c) <= -1);
//            }
//
//        SECTION("Test to see if the search function works properly.") {
//            char s = 'A';
//            char c;
//            for (int i = -64; i < 0; ++i, ++s) {
//                REQUIRE(linearHash.search(i, c) > -1);
//                REQUIRE(c == s);
//            }
//            REQUIRE(linearHash.search(64, c) <= -1);
//        }
//        SECTION("Require the print method to work properly") {
//            REQUIRE_NOTHROW(linearHash.print(std::cout));
//        }
//        SECTION("Make sure size, capacity and load all return correct values.") {
//            // The size should equal the capacity since it is full.
//            REQUIRE(linearHash.size() == 64);
//            REQUIRE(linearHash.capacity() == linearHash.size());
//            REQUIRE(linearHash.load() == 1);
//        }
//        SECTION("Check out the validity of the two new experiment methods") {
//            REQUIRE(linearHash.size() == 64);
//            REQUIRE_NOTHROW(linearHash.cluster_distribution().print_all(std::cout));
//            REQUIRE_NOTHROW(linearHash.remove_random());
//            REQUIRE(linearHash.size() == 63);
//        }
//        SECTION("Test to see that clear() works in that it clear everything.") {
//            REQUIRE_NOTHROW(linearHash.clear());
//            REQUIRE(linearHash.is_empty());
//        }
    }
}

//TEST_CASE("Open Address Hash Map: Require all basic methods to work as they are supposed to for quadratic probing", "[Quadratic Probe]") {
//    COP3530::OAHM<int, char, int (*)(int, std::size_t), hash,
//            false, int (*)(int), quad_probe> quadHashMap(65);
//
//    SECTION("Test to see that, up to the capacity of the array, the hashing and probing will allow every slot to be used") {
//        char s = 'A';
//        // With quadratic probing, I can't reach evey element without my probing function being (i*(i+1))/2, which isn't what is specified.
//        // Therefore, I search for half of the list.
//        for (int i = 0; i < 64; ++i, ++s) {
//            REQUIRE(quadHashMap.insert(i, s) > -1);
//        }
//        SECTION("Test to see that insert returns false when the array is completely full.") {
//
//
//            SECTION("Test to see that the remove function works properly.") {
//                s = 'A';
//                char c;
//                for (int i = 0; i < 64; ++i, ++s) {
//                    REQUIRE(quadHashMap.remove(i, c) > -1);
//                    REQUIRE(c == s);
//                }
//                REQUIRE(quadHashMap.remove(2, c) <= -1);
//            }
//        }
//        SECTION("Test to see if the search function works properly.") {
//            s = 'A';
//            char c;
//            for (int i = 0; i < 64; ++i, ++s) {
//                REQUIRE(quadHashMap.search(i, c) > -1);
//                REQUIRE(c == s);
//            }
//            REQUIRE(quadHashMap.search(64, c) <= -1);
//        }
//        SECTION("Require the print method to work properly") {
//            REQUIRE_NOTHROW(quadHashMap.print(std::cout));
//        }
//        SECTION("Make sure size, capacity, and load all return correct values.") {
//            // The size should equal the capacity since it is full.
//            REQUIRE(quadHashMap.size() == 64);
//            REQUIRE(quadHashMap.capacity() == quadHashMap.size() * 2);
//            REQUIRE(quadHashMap.load() == 0.5);
//        }
//        SECTION("Check out the validity of the two new experiment methods") {
//            REQUIRE(quadHashMap.size() == 64);
//            REQUIRE_NOTHROW(quadHashMap.cluster_distribution().print_all(std::cout));
//            REQUIRE_NOTHROW(quadHashMap.remove_random());
//            REQUIRE(quadHashMap.size() == 63);
//        }
//        SECTION("Test to see that clear() works in that it clear everything.") {
//            REQUIRE_NOTHROW(quadHashMap.clear());
//            REQUIRE(quadHashMap.is_empty());
//        }
//    }
//}
//
//TEST_CASE("Open Address Hash Map: Require all basic methods to work as they are supposed to for rehashing", "[Rehash]") {
//    COP3530::OAHM<int, char, int (*)(int, std::size_t), hash,
//            true, int (*)(int, std::size_t), rehash> hashHashMap(65);
//
//    SECTION("Test to see that, up to the capacity of the array, the hashing and rehashing will allow every slot to be used") {
//        char s = 'A';
//        // With quadratic probing, I can't reach evey element without my probing function being (i*(i+1))/2, which isn't what is specified.
//        // Therefore, I search for half of the list.
//        for (int i = 0; i < 64; ++i, ++s) {
//            REQUIRE(hashHashMap.insert(i, s) > -1);
//        }
//        SECTION("Test to see that insert returns false when the array is completely full.") {
//
//
//            SECTION("Test to see that the remove function works properly.") {
//                s = 'A';
//                char c;
//                for (int i = 0; i < 64; ++i, ++s) {
//                    REQUIRE(hashHashMap.remove(i, c) > -1);
//                    REQUIRE(c == s);
//                }
//                REQUIRE(hashHashMap.remove(2, c) <= -1);
//            }
//        }
//        SECTION("Test to see if the search function works properly.") {
//            s = 'A';
//            char c;
//            for (int i = 0; i < 64; ++i, ++s) {
//                REQUIRE(hashHashMap.search(i, c) > -1);
//                REQUIRE(c == s);
//            }
//            REQUIRE(hashHashMap.search(64, c) <= -1);
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
//        SECTION("Check out the validity of the two new experiment methods") {
//            REQUIRE(hashHashMap.size() == 64);
//            REQUIRE_NOTHROW(hashHashMap.cluster_distribution().print_all(std::cout));
//            REQUIRE_NOTHROW(hashHashMap.remove_random());
//            REQUIRE(hashHashMap.size() == 63);
//        }
//        SECTION("Test to see that clear() works in that it clear everything.") {
//            REQUIRE_NOTHROW(hashHashMap.clear());
//            REQUIRE(hashHashMap.is_empty());
//        }
//    }
//}
//
//TEST_CASE("Open Address Hash Map: Require all basic methods to work as they are supposed to") {
//    COP3530::BHM<int, char, int (*)(int, std::size_t), hash> myHash(128);
//
//
//    SECTION("Test to see that the remove function works properly.") {
//        char s = 'A';
//        for (int i = 0; i < 256; ++i, ++s) {
//            REQUIRE(myHash.insert(i, s) > -1);
//        }
//        s = 'A';
//        char c;
//        for (int i = 0; i < 256; ++i, ++s) {
//            REQUIRE(myHash.remove(i, c) > -1);
//            REQUIRE(c == s);
//        }
//        REQUIRE(myHash.remove(2, c) <= 1);
//    }
//
//    SECTION("Test to see that, up to over the capacity of the array, the hashing and probing will allow slots to be used") {
//        char s = 'A';
//        for (int i = 0; i < 256; ++i, ++s) {
//            REQUIRE(myHash.insert(i, s) > -1);
//        }
//        SECTION("Test to see if the search function works properly.") {
//            s = 'A';
//            char c;
//            for (int i = 0; i < 256; ++i, ++s) {
//                REQUIRE(myHash.search(i, c) > -1);
//                REQUIRE(c == s);
//            }
//            REQUIRE(myHash.search(256, c) <= -1);
//        }
//        SECTION("Require the print method to work properly") {
//            REQUIRE_NOTHROW(myHash.print(std::cout));
//        }
//        SECTION("Make sure size, capacity, and load all return correct values.") {
//            // The size should equal twice the capacity since there are twice as many elements as there are slots due to pointers.
//            REQUIRE(myHash.size() == 256);
//            REQUIRE(myHash.capacity() == myHash.size() / 2);
//            // The load should be 1 because all buckets are filled when I use 256 elements.
//            REQUIRE(myHash.load() == 1);
//        }
//        SECTION("Check out the validity of the two new experiment methods") {
//            REQUIRE(myHash.size() == 256);
//            REQUIRE_NOTHROW(myHash.cluster_distribution().print_all(std::cout));
//            REQUIRE_NOTHROW(myHash.remove_random());
//            REQUIRE(myHash.size() == 255);
//        }
//        SECTION("Test to see that clear() works in that it clear everything.") {
//            REQUIRE_NOTHROW(myHash.clear());
//            REQUIRE(myHash.is_empty());
//        }
//    }
//}