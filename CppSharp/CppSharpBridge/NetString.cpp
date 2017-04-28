#include "Stdafx.h"

#include "NetValue.h"
#include "CppNetConverter.h"

#include "CppSharpBridge.h"

using namespace System;

bool CS_NetString::IsNullOrEmpty(const CS_NetString &string) {
    return String::IsNullOrEmpty(VALUE_AS(string, System::String^));
}
bool CS_NetString::IsNullOrWhiteSpace(const CS_NetString &string) {
    return String::IsNullOrWhiteSpace(VALUE_AS(string, System::String^));
}

CS_NetString::CS_NetString() {
}
CS_NetString::CS_NetString(char *cstr) :
    CS_NetValue(CS_NetValue::String(cstr)) {
}

bool CS_NetString::operator==(const CS_NetValue &other) const {
    return Equals(other);
}
bool CS_NetString::operator!=(const CS_NetValue &other) const {
    return !Equals(other);
}

bool CS_NetString::Contains(const CS_NetString &other) {
	return VALUE_AS(*this, System::String^)->Contains(
        VALUE_AS(other, System::String^));
}

int CS_NetString::GetLength() const {
    return VALUE_AS(*this, System::String^)->Length;
}