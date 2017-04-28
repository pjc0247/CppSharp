#include "stdafx.h"

#include "CppSharpBridge.h"
#include "CppNetConverter.h"

#include "NetValue.h"

using namespace System;
using namespace System::Reflection;
using namespace System::Runtime::InteropServices;

CS_NetValue CS_NetValue::NullObject() {
    CS_NetValue value;
    value.type = CS_Object;
    value.valueData = new CS_NativeValueData(nullptr, System::Object::typeid);
    return value;
}
CS_NetValue CS_NetValue::Void() {
    CS_NetValue value;
    value.type = CS_Void;
    return value;
}
CS_NetString CS_NetValue::String(char *s) {
    CS_NetString value;
    value.type = CS_String;

    if (s == nullptr) {
        value._v.s = nullptr;
        value.valueData = new CS_NativeValueData(gcnew System::String((char*)nullptr));
    }
    else {
        value._v.s = new char[strlen(s) + 1];
        memcpy(value._v.s, s, sizeof(char) * (strlen(s) + 1));
        value.valueData = new CS_NativeValueData(gcnew System::String(s));
    }

    return value;
}

#define IMPL_CTOR(NetType, CppType, id) \
	CS_NetValue CS_NetValue::NetType(CppType id) { \
		CS_NetValue value; \
		value.type = CS_##NetType; \
		value.valueData = new CS_NativeValueData(id); \
		value._v.##id = id; \
		return value; \
	}
IMPL_CTOR(Integer, int, i);
IMPL_CTOR(Char, char, c);
IMPL_CTOR(Float, float, f);
IMPL_CTOR(Double, double, d);
IMPL_CTOR(Boolean, bool, b);

// NET_VALUE
CS_NetValue::CS_NetValue() :
    persistent(false),
	valueData(nullptr) {

	_v.s = 0;
	needToRelease = false;
}
CS_NetValue::CS_NetValue(const CS_NetValue &other) :
    persistent(false),
	type(other.type),
	valueData(nullptr), _v(other._v){

	if (other.valueData != nullptr)
		valueData = new CS_NativeValueData(other.valueData->obj);

	if (other.type == CS_String && other._v.s != nullptr) {
		_v.s = new char[strlen(other._v.s) + 1];
		memcpy(_v.s, other._v.s, sizeof(char) * (strlen(other._v.s) + 1));
	}
}
CS_NetValue::CS_NetValue(CS_NetValue &&other) :
    persistent(false),
	type(other.type),
	valueData(other.valueData), _v(other._v) {

	if (other.type == CS_String && other._v.s != nullptr) {
		_v.s = other._v.s;
		other._v.s = nullptr;
	}
	other.valueData = nullptr;
}
CS_NetValue::~CS_NetValue() {
    if (persistent) return;

	if (valueData != nullptr)
		delete valueData;

	if (needToRelease) {
		if (type == CS_String && _v.s != nullptr) {
			delete _v.s;
		}
	}
}

bool CS_NetValue::operator==(const CS_NetValue &other) const {
    return Equals(other);
}
bool CS_NetValue::operator!=(const CS_NetValue &other) const {
    return !Equals(other);
}
CS_NetValue &CS_NetValue::operator=(const CS_NetValue &other) {
	if (&other == this)
		return *this;

    Assign(other);

	if (other.valueData == nullptr)
		valueData = nullptr;
	else
		valueData = new CS_NativeValueData(other.valueData->obj);

	if (other.type == CS_String && other._v.s != nullptr) {
		_v.s = new char[strlen(other._v.s) + 1];
		memcpy(_v.s, other._v.s, sizeof(char) * (strlen(other._v.s) ));
	}

	return *this;
}
CS_NetValue &CS_NetValue::operator=(CS_NetValue &&other) {
	if (&other == this)
		return *this;

    Assign(other);

	if (other.valueData == nullptr) {
		valueData = nullptr;
	}
	else {
		valueData = other.valueData;
		other.valueData = nullptr;
	}

	if (other.type == CS_String && other._v.s != nullptr) {
		_v.s = other._v.s;
		other._v.s = nullptr;
	}

	return *this;
}

bool CS_NetValue::Equals(const CS_NetValue &other) const {
    // primitive type
    if (type != CS_Object) {
        if (other.type != CS_Object) {
            switch (type) {
            case CS_Integer: {
                int i;
                if (ToInt(other, i)) return _v.i == i;
                return false;
            }
            case CS_Char: return _v.c == other._v.c;
            case CS_Float: {
                float f;
                if (ToFloat(other, f)) return _v.f == f;
                return false;
            }
            case CS_Double: return _v.d == other._v.d;
            case CS_Boolean: return _v.b == other._v.b;
            }
        }
    }

    // string
    if (type == CS_String) {
        if (other.type != CS_String &&
            other.type != CS_Object)
            return false;

        if (_v.s == nullptr) {
            // string(null) == null
            if (other.GetType() == CS_NetType::Object() &&
                VALUE_AS(other, System::Object^) == nullptr) {
                return true;
            }
            // string(null) == string(null)
            if (other._v.s == nullptr) return true;
            else return false;
        }
        else if (other.type == CS_Object)
            return false;

        return strcmp(_v.s, other._v.s) == 0;
    }

    if (type == CS_Object) {
        // case that both values are nullptr
        if (VALUE_AS(*this, System::Object^) == nullptr &&
            VALUE_AS(other, System::Object^) == nullptr)
            return true;

        if (VALUE_AS(*this, System::Object^) == nullptr &&
            VALUE_AS(other, System::Object^) != nullptr)
            return false;
        if (VALUE_AS(*this, System::Object^) != nullptr &&
            VALUE_AS(other, System::Object^) == nullptr)
            return false;
        Console::WriteLine(valueData->obj->GetType() + " : " + other.valueData->obj->GetType());

        return valueData->obj->Equals(other.valueData->obj);
    }

    // ??? (void)
    return false;
}
CS_NetValue CS_NetValue::ToString() const {
	if (valueData == nullptr || (Object^)valueData->obj == nullptr)
		return CS_NetValue::String("nullptr");

	return ToCppValue(valueData->obj->ToString(), String::typeid);
}
const CS_NetType &CS_NetValue::GetType() const {
	switch (type) {
	case CS_Integer:
		return CS_NetType::Integer();
	case CS_Char:
		return CS_NetType::Char();
	case CS_String:
		return CS_NetType::String();
	case CS_Float:
		return CS_NetType::Float();
	case CS_Double:
		return CS_NetType::Double();
	case CS_Boolean:
		return CS_NetType::Boolean();
	}

	if (valueData != nullptr)
		return valueData->type;
}

static BindingFlags CompileFlags(bool isProperty, bool isStatic, bool isNonPublic) {
	BindingFlags flags = isProperty ? BindingFlags::GetProperty : BindingFlags::GetField;
	flags = flags | (isStatic ? BindingFlags::Static : BindingFlags::Instance);
	flags = flags | (isNonPublic ? BindingFlags::NonPublic : BindingFlags::Public);
	return flags;
}

CS_NetValue CS_NetValue::GetStaticField(const char *name, bool nonPublic) {
	auto flags = CompileFlags(false, true, nonPublic);
	auto field = valueData->GetNativeType()->GetField(
		gcnew System::String(name), flags);
	if (field == nullptr)
		return CS_NetValue::NullObject();

	return ToCppValue(field->GetValue(nullptr));
}
CS_NetValue CS_NetValue::GetField(const char *name, bool nonPublic) {
	auto flags = CompileFlags(false, false, nonPublic);
	auto field = valueData->GetNativeType()->GetField(
		gcnew System::String(name), flags);
	if (field == nullptr)
		return CS_NetValue::NullObject();

	return ToCppValue(field->GetValue(valueData->obj));
}
CS_NetValue CS_NetValue::GetStaticProperty(const char *name, bool nonPublic) {
	auto flags = CompileFlags(true, true, nonPublic);
	auto prop = valueData->GetNativeType()->GetProperty(
		gcnew System::String(name));
	if (prop == nullptr)
		return CS_NetValue::NullObject();

	return ToCppValue(prop->GetValue(nullptr));
}
CS_NetValue CS_NetValue::GetProperty(const char *name, bool nonPublic) {
	auto flags = CompileFlags(true, false, nonPublic);
	auto prop = valueData->GetNativeType()->GetProperty(
		gcnew System::String(name), flags);
	if (prop == nullptr)
		return CS_NetValue::NullObject();

	return ToCppValue(prop->GetValue(valueData->obj));
}
bool CS_NetValue::SetStaticProperty(const char *name, const CS_NetValue &value, bool nonPublic) {
	auto flags = CompileFlags(true, true, nonPublic);
	auto prop = valueData->GetNativeType()->GetProperty(
		gcnew System::String(name), flags);

	if (prop == nullptr)
		return false;

	prop->SetValue(nullptr, ToNetValue(value));
}
bool CS_NetValue::SetProperty(const char *name, const CS_NetValue &value, bool nonPublic) {
	auto flags = CompileFlags(true, false, nonPublic);
	auto prop = valueData->GetNativeType()->GetProperty(
		gcnew System::String(name), flags);

	if (prop == nullptr)
		return false;

	prop->SetValue(valueData->obj, ToNetValue(value));
}

void CS_NetValue::Assign(const CS_NetValue &other) {
    persistent = other.persistent;
    type = other.type;
    _v = other._v;
    valueData = other.valueData;
}