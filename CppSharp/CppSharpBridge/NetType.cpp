#include "stdafx.h"

#include "CppSharpBridge.h"
#include "CppNetConverter.h"

#include "NetValue.h"

using namespace System;
using namespace System::Reflection;
using namespace System::Runtime::InteropServices;

static CS_NetType _type_t(new CS_NativeTypeData(Type::typeid));
static CS_NetType _int_t(new CS_NativeTypeData(Int32::typeid));
static CS_NetType _string_t(new CS_NativeTypeData(String::typeid));
static CS_NetType _char_t(new CS_NativeTypeData(Char::typeid));
static CS_NetType _float_t(new CS_NativeTypeData(Single::typeid));
static CS_NetType _double_t(new CS_NativeTypeData(Double::typeid));
static CS_NetType _boolean_t(new CS_NativeTypeData(Boolean::typeid));

// NET_TYPE
const CS_NetType &CS_NetType::Type() {
	return _type_t;
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
	typeData(nullptr) {

	if (other.typeData == nullptr)
		return;

	typeData = new CS_NativeTypeData(other.typeData->type);
}
CS_NetType::CS_NetType(CS_NetType &&other) :
	typeData(other.typeData) {

	other.typeData = nullptr;
}
CS_NetType::CS_NetType(CS_NativeTypeData *_data) {
	typeData = _data;

	valueData = new CS_NativeValueData(nullptr, *this);
}
CS_NetType::~CS_NetType() {
	if (typeData != nullptr)
		delete typeData;
}

CS_NetType &CS_NetType::operator=(const CS_NetType &other) {
	if (&other == this)
		return *this;

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

	CS_NetValue::operator=(std::move(other));

	if (other.typeData == nullptr)
		typeData = nullptr;
	else {
		typeData = other.typeData;
		other.typeData = nullptr;
	}

	return *this;
}

CS_NetValue CS_NetType::ToString() {
	return ToCppValue(typeData->type->Name);
}
