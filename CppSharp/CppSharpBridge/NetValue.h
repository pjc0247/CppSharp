#pragma once

#include <msclr/auto_gcroot.h>

#include "CppSharpBridge.h"

struct CS_NativeTypeData {
	msclr::gcroot<System::Type^> type;

	CS_NativeTypeData(System::Type ^type) :
		type(type){
	}
	~CS_NativeTypeData() {
	}
};

struct CS_NativeValueData {
	msclr::auto_gcroot<System::Object^> obj;

	CS_NetType type;

	CS_NativeValueData(System::Object ^obj) {
		this->obj = obj;

		if (obj == nullptr)
			this->type = CS_NetType(new CS_NativeTypeData(void::typeid));
		else
			this->type = CS_NetType(new CS_NativeTypeData(obj->GetType()));
	}
};

