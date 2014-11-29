#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "OAHM.h"

inline int hash(int key, std::size_t m) {
    const double A = (sqrt(5) - 1) / 2;
    double intPart;
    return (int)(m * modf(A * key, &intPart));
}
inline int quad_probe(int i) { return i; }

TEST_CASE("Open Address Hash Map: Require all basic methods to work as they are supposed to") {
    COP3530::OAHM<int, char, int (*)(int, std::size_t), hash,
            false, int (*)(int), quad_probe> myHash(65);

    SECTION("Test to see that, up to the capacity of the array, the hashing and probing will allow every slot to be used") {
        char s = 'A';
        // Capacity of the array is always a power of 2. 64 is the power of 2 greater than but closest to the size 33 specified.
        for (int i = 0; i < 128; ++i, ++s) {
            REQUIRE(myHash.insert(i, s));
        }
        SECTION("Test to see that insert returns false when the array is completely full.") {
            REQUIRE(!myHash.insert(11, s));


            SECTION("Test to see that the remove function works properly.") {
                s = 'A';
                char c;
                for (int i = 0; i < 128; ++i, ++s) {
                    REQUIRE(myHash.remove(i, c));
                    REQUIRE(c == s);
                }
                REQUIRE(!myHash.remove(2, c));
            }
        }
        SECTION("Test to see if the search function works properly.") {
            s = 'A';
            char c;
            for (int i = 0; i < 128; ++i, ++s) {
                REQUIRE(myHash.search(i, c));
                REQUIRE(c == s);
            }
            REQUIRE(!myHash.search(128, c));
        }
        SECTION("Require the print method to work properly") {
            REQUIRE_NOTHROW(myHash.print(std::cout));
        }
        SECTION("Make sure size, capacity, and load all return correct values.") {
            // The size should equal the capacity since it is full.
            REQUIRE(myHash.size() == 128);
            REQUIRE(myHash.capacity() == myHash.size());
            REQUIRE(myHash.load() == 1);
        }
        SECTION("Test to see that clear() works in that it clear everything.") {
            REQUIRE_NOTHROW(myHash.clear());
            REQUIRE(myHash.is_empty());
        }
    }
}
