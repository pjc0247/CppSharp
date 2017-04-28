#pragma once

#include "CppSharpBridge.h"

#define VALUE_AS(x, type) ((type)((x).valueData->GetNativeObject()))

System::Object ^ToNetValue(const CS_NetValue &obj);
CS_NetValue ToCppValue(System::Object ^obj);
CS_NetValue ToCppValue(System::Object ^obj, System::Type ^type);

// to primitive types
bool ToInt(const CS_NetValue &obj, int &v);
bool ToFloat(const CS_NetValue &obj, float &v);