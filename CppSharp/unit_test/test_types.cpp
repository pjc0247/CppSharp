#include "stdafx.h"
#include "catch.hpp"

#include "CppSharpBridge.h"

TEST_CASE("Types") {
    SECTION("CompareType") {
        REQUIRE(CS_NetValue::Integer(0).GetType() == CS_NetType::Integer());
        REQUIRE(CS_NetValue::Char(0).GetType() == CS_NetType::Char());
        REQUIRE(CS_NetValue::Float(0).GetType() == CS_NetType::Float());
        REQUIRE(CS_NetValue::Double(0).GetType() == CS_NetType::Double());
        REQUIRE(CS_NetValue::Boolean(0).GetType() == CS_NetType::Boolean());
        REQUIRE(CS_NetValue::String("").GetType() == CS_NetType::String());
        REQUIRE(CS_NetValue::NullObject().GetType() == CS_NetType::Object());
    }
    SECTION("TypeType") {
        REQUIRE(CS_NetValue::NullObject().GetType().GetType()
                == CS_NetType::Type());
    }
}
