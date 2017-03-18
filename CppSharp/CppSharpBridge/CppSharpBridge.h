#pragma once

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

struct CS_Script {
	char *path;
	char *source;
};
struct CS_NetValue {
	CS_NetValueType type;
	
	union {
		int i;
		char c;
		char *s;
		float f;
		double d;
		bool b;
	};

	bool needToRelease;

	CPPSHARP_API static CS_NetValue Void();
	CPPSHARP_API static CS_NetValue Integer(int i);
	CPPSHARP_API static CS_NetValue Char(char c);
	CPPSHARP_API static CS_NetValue String(char *s);
	CPPSHARP_API static CS_NetValue Float(float f);
	CPPSHARP_API static CS_NetValue Double(double d);
	CPPSHARP_API static CS_NetValue Boolean(bool d);

	CPPSHARP_API CS_NetValue();
	CPPSHARP_API ~CS_NetValue();
};

CPPSHARP_API void CS_LoadScripts(CS_Script *scripts, int len);

CPPSHARP_API void CS_RegisterFunction(const char *name, CS_NetValue (*func)(CS_NetValue *,int));
CPPSHARP_API CS_NetValue CS_Invoke(const char *name, CS_NetValue *args, int argc);