#pragma once

#include <stdio.h>
#include <stdarg.h>

#if CppSharpExport
#define CPPSHARP_API __declspec(dllexport)
#else
#define CPPSHARP_API __declspec(dllimport)
#endif

#define _AS_INT(args, n) \
	(args[n].type == CS_Integer ? args[n].i : -1)

enum CS_NetValueType {
	CS_Void,
	CS_Integer,
	CS_Char,
	CS_String,
	CS_Float,
	CS_Double,
	CS_Boolean
};

struct CS_NativeValueData;
struct CS_NativeTypeData;

struct CS_Script {
	char *path;
	char *source;
};
class CPPSHARP_API CS_NetType{
public:
	CS_NativeTypeData *data;

	static CS_NetType Integer();
	static CS_NetType String();
	static CS_NetType Char();
	static CS_NetType Float();
	static CS_NetType Double();
	static CS_NetType Boolean();

	CS_NetType();
	CS_NetType(CS_NativeTypeData*);
	CS_NetType(const CS_NetType &other);
	~CS_NetType();

	CS_NetType &operator=(const CS_NetType &other);
};
class CPPSHARP_API CS_NetValue {
public:
	CS_NativeValueData *data;
	CS_NetValueType type;
	
	// primitive data
	union {
		int i;
		char c;
		char *s;
		float f;
		double d;
		bool b;
	} _v;

	bool needToRelease;

	static CS_NetValue Void();
	static CS_NetValue Integer(int i);
	static CS_NetValue Char(char c);
	static CS_NetValue String(char *s);
	static CS_NetValue Float(float f);
	static CS_NetValue Double(double d);
	static CS_NetValue Boolean(bool d);

	CS_NetValue();
	CS_NetValue(const CS_NetValue &other);
	~CS_NetValue();

	const CS_NetType &GetType();
};

CPPSHARP_API void CS_LoadScripts(CS_Script *scripts, int len);

CPPSHARP_API void CS_RegisterFunction(
	const char *name, CS_NetValue (*func)(...),
	const CS_NetType &returnType,
	CS_NetType *paramTypes, int paramTypesLen);
CPPSHARP_API CS_NetValue CS_Invoke(const char *name, CS_NetValue *args, int argc);