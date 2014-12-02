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
    COP3530::RBST<std::string, char> strTree(10);
    COP3530::RBST<char*, char> charTree(10);
    COP3530::RBST<double, char> dubTree(10);

    SECTION("Test to see that, up to the capacity of the array, you can insert properly") {
        char s = 'A';
        double d = 0.5;
        char* strang = "hahahaha";
        char* twoStrang = "hehehehehe";
        std::string str1 = "This is first";
        std::string str2 = "This is second";
        REQUIRE(strTree.insert(str1, 'A') > -1);
        REQUIRE(strTree.insert(str2, 'B') > -1);
        REQUIRE(charTree.insert(strang, 'A') > -1);
        REQUIRE(charTree.insert(twoStrang, 'B') > -1);
        for (int i = 0; i < 10; ++i, ++s, ++d) {
            REQUIRE(tree.insert(i, s) > -1);
            REQUIRE(dubTree.insert(d, s) > -1);
        }
        REQUIRE(tree.insert(100, s) <= -1);
        REQUIRE(dubTree.insert(0.8, s) <= -1);


        SECTION("Test out the search function for all of the variants, and also print") {
            tree.print(std::cout);
            strTree.print(std::cout);
            REQUIRE(strTree.search(str1, s) > -1);
            REQUIRE(strTree.search(str2, s) > -1);
            REQUIRE(charTree.search(strang, s) > -1);
            REQUIRE(charTree.search(twoStrang, s) > -1);
            s = 'A';
            char c;
            double d = 0.5;
            for (int i = 0; i < 10; ++i, ++s, ++d) {
                REQUIRE(tree.search(i, c) > -1);
                REQUIRE(c == s);
                REQUIRE(dubTree.search(d, c) > -1);
                REQUIRE(c == s);
            }
        }

        SECTION("Test out the remove function for all of the variants") {
            REQUIRE(strTree.remove(str1, s) > -1);
            REQUIRE(strTree.remove(str2, s) > -1);
            REQUIRE(charTree.remove(strang, s) > -1);
            REQUIRE(charTree.remove(twoStrang, s) > -1);
            s = 'A';
            d = 0.5;
            for (int i = 0; i < 10; ++i, ++s, ++d) {
                REQUIRE(tree.remove(i, s) > -1);
                REQUIRE(dubTree.remove(d, s) > -1);
            }

            SECTION("Test out the size, is_empty, load and capacity functions.") {
                REQUIRE(tree.size() == 0);
                REQUIRE(tree.is_empty());
                REQUIRE(tree.load() == 0);
                REQUIRE(tree.capacity() == 10);
            }
        }

        SECTION("Test out the clear function") {
            REQUIRE(!tree.is_empty());
            REQUIRE_NOTHROW(tree.clear());
            REQUIRE(tree.is_empty());
        }
    }
}
