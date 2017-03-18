#include "stdafx.h"

#include "CppSharpBridge.h"

using namespace System;
using namespace System::Runtime::InteropServices;

#define IMPL_CTOR(NetType, CppType, id) \
	CS_NetValue CS_NetValue::NetType(CppType id) { \
		CS_NetValue value; \
		value.type = CS_##NetType; \
		value.##id = id; \
		return value; \
	}

CS_NetValue::CS_NetValue() {
	needToRelease = false;
}
CS_NetValue::~CS_NetValue() {
	if (needToRelease) {
		if (type == CS_String)
			Marshal::FreeHGlobal((IntPtr)s);
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