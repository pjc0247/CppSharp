#include "stdafx.h"

#include "CppSharpBridge.h"
#include "CppNetConverter.h"

#include "NetValue.h"

using namespace System;
using namespace System::Reflection;
using namespace System::Runtime::InteropServices;

static CS_NetType _type_t(new CS_NativeTypeData(Type::typeid), true);
static CS_NetType _object_t(new CS_NativeTypeData(Object::typeid), true);
static CS_NetType _int_t(new CS_NativeTypeData(Int32::typeid), true);
static CS_NetType _string_t(new CS_NativeTypeData(String::typeid), true);
static CS_NetType _char_t(new CS_NativeTypeData(Char::typeid), true);
static CS_NetType _float_t(new CS_NativeTypeData(Single::typeid), true);
static CS_NetType _double_t(new CS_NativeTypeData(Double::typeid), true);
static CS_NetType _boolean_t(new CS_NativeTypeData(Boolean::typeid), true);

// NET_TYPE
const CS_NetType &CS_NetType::Type() {
	return _type_t;
}
const CS_NetType &CS_NetType::Object() {
    return _object_t;
}
const CS_NetType &CS_NetType::Integer() {
	return _int_t;
}
const CS_NetType &CS_NetType::String() {
	return _string_t;
}
const CS_NetType &CS_NetType::Char() {
	return _char_t;
}
const CS_NetType &CS_NetType::Float() {
	return _float_t;
}
const CS_NetType &CS_NetType::Double() {
	return _double_t;
}
const CS_NetType &CS_NetType::Boolean() {
	return _boolean_t;
}
bool CS_NetType::ParseFromName(const char *name, CS_NetType &_out) {
	auto netType = Type::GetType(gcnew System::String(name));

	if (netType == nullptr)
		return false;

	_out = CS_NetType(new CS_NativeTypeData(netType));
	return true;
}

CS_NetType::CS_NetType() :
	typeData(nullptr) {
}
CS_NetType::CS_NetType(const CS_NetType &other) :
    CS_NetValue(other),
	typeData(nullptr) {

	if (other.typeData == nullptr)
		return;

	typeData = new CS_NativeTypeData(other.typeData->type);
}
CS_NetType::CS_NetType(CS_NetType &&other) :
    CS_NetValue(other),
	typeData(other.typeData) {

	other.typeData = nullptr;
}
CS_NetType::CS_NetType(CS_NativeTypeData *_data, bool _persistent) {
    persistent = _persistent;

    type = CS_Object;
    typeData = _data;
	valueData = new CS_NativeValueData(nullptr, *this);
}
CS_NetType::~CS_NetType() {
    if (persistent) return;

	if (typeData != nullptr)
		delete typeData;
}

bool CS_NetType::operator==(const CS_NetType &other) const {
    return Equals(other);
}
bool CS_NetType::operator!=(const CS_NetType &other) const {
    return !Equals(other);
}
CS_NetType &CS_NetType::operator=(const CS_NetType &other) {
	if (&other == this)
		return *this;

    if (other.persistent) {
        _v = other._v;
        persistent = other.persistent;
        type = other.type;
        valueData = other.valueData;
        typeData = other.typeData;
        return *this;
    }

	CS_NetValue::operator=(other);

	if (other.typeData == nullptr)
		typeData = nullptr;
	else {
		typeData = new CS_NativeTypeData(other.typeData->type);
	}

	return *this;
}
CS_NetType &CS_NetType::operator=(CS_NetType &&other) {
	if (&other == this)
		return *this;

    if (other.persistent) {
        _v = other._v;
        persistent = other.persistent;
        type = other.type;
        valueData = other.valueData;
        typeData = other.typeData;
        return *this;
    }

	CS_NetValue::operator=(std::move(other));

	if (other.typeData == nullptr)
		typeData = nullptr;
	else {
		typeData = other.typeData;
		other.typeData = nullptr;
	}

	return *this;
}

CS_NetValue CS_NetType::ToString() const {
	return ToCppValue(typeData->type->Name);
}

void CS_NetType::Assign(const CS_NetType &other) {
    CS_NetValue::Assign(other);
    typeData = other.typeData;
}