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
	msclr::gcroot<System::Object^> obj;

	CS_NetType type;

	CS_NativeValueData(System::Object ^obj) {
		this->obj = obj;

		if (obj == nullptr)
			this->type = CS_NetType(new CS_NativeTypeData(void::typeid));
		else
			this->type = CS_NetType(new CS_NativeTypeData(obj->GetType()));
	}
	CS_NativeValueData(System::Object ^obj, System::Type ^type) {
		this->obj = obj;

		if (type == System::Type::typeid)
			this->type = CS_NetType::Type();
		else {
			
			this->type = (new CS_NativeTypeData(type));
		}
	}
	CS_NativeValueData(System::Object ^obj, const CS_NetType &type) {
		this->obj = obj;
		this->type = type;
	}

	System::Type ^GetNativeType() const {
		return type.typeData->type;
	}
};

