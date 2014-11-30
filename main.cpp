#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "BHM.h"

inline int hash(int key, std::size_t m) {
    const double A = (sqrt(5) - 1) / 2;
    double intPart;
    return (int)(m * modf(A * key, &intPart));
}

TEST_CASE("Open Address Hash Map: Require all basic methods to work as they are supposed to") {
    COP3530::BHM<int, char, int (*)(int, std::size_t), hash> myHash(128);


    SECTION("Test to see that the remove function works properly.") {
        char s = 'A';
        for (int i = 0; i < 256; ++i, ++s) {
            REQUIRE(myHash.insert(i, s));
        }
        s = 'A';
        char c;
        for (int i = 0; i < 256; ++i, ++s) {
            REQUIRE(myHash.remove(i, c));
            REQUIRE(c == s);
        }
        REQUIRE(!myHash.remove(2, c));
    }

    SECTION("Test to see that, up to the capacity of the array, the hashing and probing will allow every slot to be used") {
        char s = 'A';
        // Capacity of the array is always a power of 2. 64 is the power of 2 greater than but closest to the size 33 specified.
        for (int i = 0; i < 256; ++i, ++s) {
            REQUIRE(myHash.insert(i, s));
        }
        SECTION("Test to see if the search function works properly.") {
            s = 'A';
            char c;
            for (int i = 0; i < 256; ++i, ++s) {
                REQUIRE(myHash.search(i, c));
                REQUIRE(c == s);
            }
            REQUIRE(!myHash.search(256, c));
        }
        SECTION("Require the print method to work properly") {
            REQUIRE_NOTHROW(myHash.print(std::cout));
        }
        SECTION("Make sure size, capacity, and load all return correct values.") {
            // The size should equal twice the capacity since there are twice as many elements as there are slots due to pointers.
            REQUIRE(myHash.size() == 256);
            REQUIRE(myHash.capacity() == myHash.size() / 2);
            // The load should be 1 because all slots are filled when I use 256 elements.
            REQUIRE(myHash.load() == 1);
        }
        SECTION("Test to see that clear() works in that it clear everything.") {
            REQUIRE_NOTHROW(myHash.clear());
            REQUIRE(myHash.is_empty());
        }
    }
}
