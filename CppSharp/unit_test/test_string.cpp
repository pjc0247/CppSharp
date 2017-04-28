#include "stdafx.h"
#include "catch.hpp"

#include "CppSharpBridge.h"

TEST_CASE("NetString") {
    SECTION("FromCstr") {
        auto str1 = CS_NetString("hello");

        REQUIRE(strcmp("hello", str1._v.s) == 0);

        auto str2 = CS_NetString("hello");
        auto str3 = CS_NetString("bye");

        REQUIRE(str1 == str2);
        REQUIRE(str1 != str3);
    }

    SECTION("IsNullOrEmpty") {
        REQUIRE(CS_NetString::IsNullOrEmpty(CS_NetValue::String(nullptr)) == true);
        REQUIRE(CS_NetString::IsNullOrEmpty(CS_NetValue::String("")) == true);
        REQUIRE(CS_NetString::IsNullOrEmpty(CS_NetValue::String("hello")) == false);

        REQUIRE(CS_NetString::IsNullOrWhiteSpace(CS_NetValue::String(nullptr)) == true);
        REQUIRE(CS_NetString::IsNullOrWhiteSpace(CS_NetValue::String("")) == true);
        REQUIRE(CS_NetString::IsNullOrWhiteSpace(CS_NetValue::String("   ")) == true);
        REQUIRE(CS_NetString::IsNullOrWhiteSpace(CS_NetValue::String("hello")) == false);
    }

    SECTION("Compare") {
        REQUIRE(CS_NetString("aa") == CS_NetValue::String("aa"));
        REQUIRE(CS_NetString("aa") != CS_NetValue::String("bb"));

        REQUIRE(CS_NetString("aa") == CS_NetString("aa"));
        REQUIRE(CS_NetString("aa") != CS_NetString("bb"));
    }
}
