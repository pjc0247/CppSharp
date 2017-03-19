#pragma once

#include "CppSharpBridge.h"

System::Object ^ToNetValue(const CS_NetValue &obj);
CS_NetValue ToCppValue(System::Object ^obj);
CS_NetValue ToCppValue(System::Object ^obj, System::Type ^type);