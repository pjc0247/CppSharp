#include "stdafx.h"

#include "CppSharpBridge.h"

#include "NetValue.h"

using namespace System;
using namespace System::Runtime::InteropServices;

#define IMPL_CTOR(NetType, CppType, id) \
	CS_NetValue CS_NetValue::NetType(CppType id) { \
		CS_NetValue value; \
		value.type = CS_##NetType; \
		value._v.##id = id; \
		return value; \
	}

// NET_TYPE
 CS_NetType CS_NetType::Integer() {
	return CS_NetType(new CS_NativeTypeData(Int32::typeid));
}
CS_NetType CS_NetType::String() {
	return CS_NetType(new CS_NativeTypeData(String::typeid));
}
CS_NetType CS_NetType::Char() {
	return CS_NetType(new CS_NativeTypeData(Char::typeid));
}
CS_NetType CS_NetType::Float() {
	return CS_NetType(new CS_NativeTypeData(Single::typeid));
}
CS_NetType CS_NetType::Double() {
	return CS_NetType(new CS_NativeTypeData(Double::typeid));
}
CS_NetType CS_NetType::Boolean() {
	return CS_NetType(new CS_NativeTypeData(Boolean::typeid));
}

CS_NetType::CS_NetType() :
	data(nullptr) {
}
CS_NetType::CS_NetType(const CS_NetType &other) :
	data(nullptr){

	if (other.data == nullptr)
		return;

	data = new CS_NativeTypeData(other.data->type);
}
CS_NetType::CS_NetType(CS_NativeTypeData *_data) {
	data = _data;
}
CS_NetType::~CS_NetType() {
	if (data != nullptr)
		delete data;
}

CS_NetType &CS_NetType::operator=(const CS_NetType &other) {
	if (&other == this)
		return *this;

	if (other.data == nullptr)
		data = nullptr;
	else {
		data = new CS_NativeTypeData(other.data->type);
	}

	return *this;
}

// NET_VALUE
CS_NetValue::CS_NetValue() :
	data(nullptr){
	needToRelease = false;
}
CS_NetValue::CS_NetValue(const CS_NetValue &other) :
	type(other.type),
	data(nullptr), _v(other._v){

	if (other.data == nullptr)
		return;

	data = new CS_NativeValueData(other.data->obj.get());
}
CS_NetValue::~CS_NetValue() {
	if (data != nullptr)
		delete data;

	if (needToRelease) {
		if (type == CS_String)
			Marshal::FreeHGlobal((IntPtr)_v.s);
	}
}

CS_NetValue CS_NetValue::Void() {
	CS_NetValue value;
	value.type = CS_Void;
	return value;
}

IMPL_CTOR(Integer, int, i);
IMPL_CTOR(Char, char, c);
IMPL_CTOR(String, char*, s);
IMPL_CTOR(Float, float, f);
IMPL_CTOR(Double, double, d);
IMPL_CTOR(Boolean, bool, b);

const CS_NetType &CS_NetValue::GetType() {
	if (data != nullptr)
		return data->type;
}