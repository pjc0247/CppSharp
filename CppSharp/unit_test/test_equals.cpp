#include "stdafx.h"
#include "catch.hpp"

#include "CppSharpBridge.h"

TEST_CASE("Equals") {
    // All methods must contains tests below.
    //   * `Equals` with same type same value
    //   * 'Equals` with same type different value
    //   * `Equals` with `nullptr` value (If object)
    //   * `Equals` with `String` type
    //   * `Equals` with 'Object` type

	SECTION("Integer") {
        REQUIRE(CS_NetValue::Integer(1234) == CS_NetValue::Integer(1234));

        REQUIRE(CS_NetValue::Integer(0) != CS_NetValue::Integer(1));
        REQUIRE(CS_NetValue::Integer(0) != CS_NetValue::String(""));
        REQUIRE(CS_NetValue::Integer(0) != CS_NetValue::NullObject());
	}
    SECTION("Char") {
        REQUIRE(CS_NetValue::Char(1234) == CS_NetValue::Char(1234));

        REQUIRE(CS_NetValue::Char(0) != CS_NetValue::Char(1));
        REQUIRE(CS_NetValue::Char(0) != CS_NetValue::String(""));
        REQUIRE(CS_NetValue::Char(0) != CS_NetValue::NullObject());
    }
    SECTION("Float") {
        REQUIRE(CS_NetValue::Float(1.1) == CS_NetValue::Float(1.1));

        REQUIRE(CS_NetValue::Float(0) != CS_NetValue::Float(1));
        REQUIRE(CS_NetValue::Float(0) != CS_NetValue::String(""));
        REQUIRE(CS_NetValue::Float(0) != CS_NetValue::NullObject());
    }
    SECTION("String") {
        REQUIRE(CS_NetValue::String("hello world") == CS_NetValue::String("hello world"));
        REQUIRE(CS_NetValue::String(nullptr) == CS_NetValue::String(nullptr));
        REQUIRE(CS_NetValue::String(nullptr) == CS_NetValue::NullObject());

        REQUIRE(CS_NetValue::String("bye world") != CS_NetValue::String("hello world"));
        REQUIRE(CS_NetValue::String("bye world") != CS_NetValue::Integer(1));
        REQUIRE(CS_NetValue::String("hello world") != CS_NetValue::NullObject());
    }
};
