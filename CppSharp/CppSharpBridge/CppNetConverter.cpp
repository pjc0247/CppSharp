#include "stdafx.h"

#include "CppSharpBridge.h"
#include "NetValue.h"

#include "CppNetConverter.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Object ^ToNetValue(const CS_NetValue &obj) {
	Console::WriteLine("ToNetValue");

	if (obj.type == CS_Void)
		return nullptr;

	if (obj.type == CS_Integer)
		return obj._v.i;
	if (obj.type == CS_Char)
		return obj._v.c;
	if (obj.type == CS_Float)
		return obj._v.f;
	if (obj.type == CS_Double)
		return obj._v.d;
	if (obj.type == CS_Boolean)
		return obj._v.b;
}

CS_NetValue ToCppValue(Object ^obj) {
	if (obj != nullptr)
		return ToCppValue(obj, obj->GetType());
	else
		return ToCppValue(obj, Object::typeid);
}
CS_NetValue ToCppValue(Object ^obj, Type ^type) {
	if (type == Void::typeid)
		return CS_NetValue::Void();

	if (type == Int32::typeid)
		return CS_NetValue::Integer((Int32)obj);
	if (type == Char::typeid)
		return CS_NetValue::Char((Char)obj);
	if (type == Single::typeid)
		return CS_NetValue::Float((Single)obj);
	if (type == Double::typeid)
		return CS_NetValue::Double((Double)obj);
	if (type == Boolean::typeid)
		return CS_NetValue::Boolean((Boolean)obj);

	if (type == String::typeid) {
		if (obj == nullptr)
			return CS_NetValue::String(nullptr);

		auto cstr = (char*)(Marshal::StringToHGlobalAnsi((String^)obj).ToPointer());
		auto v = CS_NetValue::String(cstr);
		Marshal::FreeHGlobal((IntPtr)cstr);
		return v;
	}

	CS_NetValue value;
	value.type = CS_Object;
	value.valueData = new CS_NativeValueData(obj);
	return value;
}

bool ToInt(const CS_NetValue &obj, int &v) {
    if (obj.type == CS_Integer)
        v =  obj._v.i;
    if (obj.type == CS_Float)
        v = obj._v.f;
    if (obj.type == CS_Double)
        v = obj._v.d;
    if (obj.type == CS_Char)
        v = obj._v.c;

    if (obj.type == CS_Boolean || obj.type == CS_String)
        return false;
    // TOODO
    if (obj.type == CS_Object)
        return false;

    return true;
}
bool ToFloat(const CS_NetValue &obj, float &v) {
    if (obj.type == CS_Integer)
        v = obj._v.i;
    if (obj.type == CS_Float)
        v = obj._v.f;
    if (obj.type == CS_Double)
        v = obj._v.d;

    if (obj.type == CS_Boolean || obj.type == CS_String || 
        obj.type == CS_Char)
        return false;
    if (obj.type == CS_Object)
        return false;

    return true;
}