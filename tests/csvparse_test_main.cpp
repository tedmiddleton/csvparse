
//          Copyright Ted Middleton 2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <string>

#include <csvparse/parse.hpp>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "date.h"
#include "debug_cout.hpp"


TEST_CASE("basic", "[csvparse]")
{
const char teststr[] =
R"(Year,Make,Model,Description,Price
1997,Ford,E350,"ac, abs, moon",3000.00
1999,Chevy,"Venture ""Extended Edition""","",4900.00
1999,Chevy,"Venture ""Extended Edition, Very Large""","",5000.00
1996,Jeep,Grand Cherokee,"MUST SELL!
air, moon roof, loaded",4799.00)";

    auto endrecord = [](auto n) { std::cout << "End record: " << n << "\n"; };
    auto endfield = [](auto n) { std::cout << "End field: " << n << "\n"; };
    SECTION( "allow lf" )
    {
        csvparse::parser parser;
        std::string test{ teststr };
        for (int ch : test) {
            parser.advance(ch, endfield, endrecord);
        }
        REQUIRE(!parser.is_error());
    }

    SECTION( "disallow lf" )
    {
        csvparse::parser<',', false> parser;
        std::string test{ teststr };
        for (int ch : test) {
            parser.advance(ch, endfield, endrecord);
        }
        REQUIRE(parser.is_error());
    }

    SECTION( "semicolon" )
    {
        csvparse::parser<';'> parser;
        std::string test{ teststr };
        for (int ch : test) {
            parser.advance(ch, endfield, endrecord);
        }
        REQUIRE(parser.is_error());
    }
}

TEST_CASE("eu", "[csvparse]")
{
const char teststr[] =
R"(Year;Make;Model;Description;Price
1997;Ford;E350;"ac; abs, moon";3000,00
1999;Chevy;"Venture ""Extended Edition""";"";4900,00
1999;Chevy;"Venture ""Extended Edition; Very Large""";"";5000,00
1996;Jeep;Grand Cherokee;"MUST SELL!
air; moon roof, loaded";4799,00)";

    auto endrecord = [](auto n) { std::cout << "End record: " << n << "\n"; };
    auto endfield = [](auto n) { std::cout << "End field: " << n << "\n"; };
    SECTION( "semicolon" )
    {
        csvparse::parser<';'> parser;
        std::string test{ teststr };
        for (int ch : test) {
            parser.advance(ch, endfield, endrecord);
        }
        REQUIRE(!parser.is_error());
    }

    SECTION( "default" )
    {
        csvparse::parser parser;
        std::string test{ teststr };
        for (int ch : test) {
            parser.advance(ch, endfield, endrecord);
        }
        REQUIRE(parser.is_error());
    }
}

