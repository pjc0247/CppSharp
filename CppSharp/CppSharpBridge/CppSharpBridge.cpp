#include "stdafx.h"

#include "NetValue.h"
#include "CppSharpBridge.h"

#include <msclr/marshal_cppstd.h>
#include <msclr/auto_gcroot.h>
#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <functional>

using namespace System;
using namespace System::Reflection;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace CppSharp;

static msclr::auto_gcroot<ExeContext^> exeContext;

static std::map < std::string, std::function<CS_NetValue(CS_NetValue*, int)>> functions;

static CS_NetValue ToCppValue(Object ^obj);
static Object ^ToNetValue(const CS_NetValue &obj);

public ref class CSFunctionBindings {
public:
	static Object ^Invoke(String ^name, array<Object^> ^args) {
		Console::WriteLine("CSFunctionBindings::Invoke : {0}", name);

		if (args == nullptr)
			args = gcnew array<Object^>(0);

		auto cppName = msclr::interop::marshal_as<std::string>(name);
		auto it = functions.find(cppName);

		if (it == functions.end()) {
			Console::WriteLine("not found function - {0}", name);
			throw gcnew ArgumentException(name);
		}

		CS_NetValue *cppArgs = new CS_NetValue[args->Length];
		for (int i = 0; i < args->Length;i++) {
			cppArgs[i] = ToCppValue(args[i]);
		}

		auto ret = (*it).second(cppArgs, args->Length);
		 
		return ToNetValue(ret);
	}

private:
};

void CS_LoadScripts(CS_Script *scripts, int len) {
	auto input = gcnew List<Tuple<String^, String^>^>();

	for (int i = 0; i < len; i++) {
		input->Add(gcnew Tuple<String^, String^>(
			gcnew String(scripts[i].path),
			gcnew String(scripts[i].source)));
	}

	exeContext = CSCore::LoadScripts(
		Assembly::GetCallingAssembly(),
		input->ToArray());
}

static Object ^ToNetValue(const CS_NetValue &obj) {
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
static CS_NetValue ToCppValue(Object ^obj) {
	auto objType = obj->GetType();

	if (objType == Void::typeid)
		return CS_NetValue::Void();
	
	if (objType == Int32::typeid)
		return CS_NetValue::Integer((Int32)obj);
	if (objType == Char::typeid)
		return CS_NetValue::Char((Char)obj);
	if (objType == Single::typeid)
		return CS_NetValue::Float((Single)obj);
	if (objType == Double::typeid)
		return CS_NetValue::Double((Double)obj);
	if (objType == Boolean::typeid)
		return CS_NetValue::Boolean((Boolean)obj);

	if (objType == String::typeid) {
		auto cstr = (char*)(Marshal::StringToHGlobalAnsi((String^)obj).ToPointer());
		auto v = CS_NetValue::String(cstr);
		v.needToRelease = true;
		return v;
	}
}

void CS_RegisterFunction(
	const char *name, CS_NetValue(*func)(...),
	const CS_NetType &returnType,
	CS_NetType *paramTypes, int paramTypesLen) {

	//if (functions.find(std::string(name)) != functions.end())
	//	printf("already has a function with name(%s), override it.", name);

	auto paramTypesList = gcnew List<Type^>();
	for (int i = 0; i < paramTypesLen; i++) {
		paramTypesList->Add(paramTypes[i].data->type);
	}

	CSCore::RegisterFunction(
		gcnew String(name),
		returnType.data->type,
		paramTypesList->ToArray());

	functions[std::string(name)] = [func](CS_NetValue *args, int argc) {
		if (argc == 0)
			return func();
		else if (argc == 1)
			return func(args[0]);
		else if (argc == 2)
			return func(args[0], args[1]);
		else if (argc == 3)
			return func(args[0], args[1], args[2]);
		else if (argc == 4)
			return func(args[0], args[1], args[2], args[3]);
	};
}
CS_NetValue CS_Invoke(const char *name, CS_NetValue *args, int argc) {
	auto proxy = exeContext.get()->GetProxy();
	auto param = gcnew List<Object^>();

	for (int i = 0; i < argc; i++) {
		switch (args[i].type) {
		case CS_Integer:
			param->Add(args[i]._v.i);
			break;
		case CS_Char:
			param->Add(args[i]._v.c);
			break;
		case CS_String:
			param->Add(gcnew String(args[i]._v.s));
			break;
		case CS_Float:
			param->Add(args[i]._v.f);
			break;
		case CS_Double:
			param->Add(args[i]._v.d);
			break;
		case CS_Boolean:
			param->Add(args[i]._v.b);
			break;
		}
	}

	auto ret = proxy->Invoke(gcnew String(name), param->ToArray());

	return ToCppValue(ret);
}