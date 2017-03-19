#pragma once

#include <stdio.h>
#include <stdarg.h>

#if CppSharpExport
#define CPPSHARP_API __declspec(dllexport)
#else
#define CPPSHARP_API __declspec(dllimport)
#endif

enum CS_NetValueType {
	CS_Void,
	CS_Integer,
	CS_Char,
	CS_String,
	CS_Float,
	CS_Double,
	CS_Boolean,
	CS_Object
};

struct CS_NativeValueData;
struct CS_NativeTypeData;
class CS_NetType;

struct CS_Script {
	char *path;
	char *source;
};

class CPPSHARP_API CS_NetValue {
public:
	CS_NativeValueData *valueData;
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

	static CS_NetValue NullObject();
	static CS_NetValue Void();
	static CS_NetValue Integer(int i);
	static CS_NetValue Char(char c);
	static CS_NetValue String(char *s);
	static CS_NetValue Float(float f);
	static CS_NetValue Double(double d);
	static CS_NetValue Boolean(bool d);

	CS_NetValue();
	CS_NetValue(const CS_NetValue &other);
	CS_NetValue(CS_NetValue &&other);
	virtual ~CS_NetValue();

	virtual CS_NetValue &operator=(const CS_NetValue &other);
	virtual CS_NetValue &operator=(CS_NetValue &&other);

	virtual CS_NetValue ToString();
	const CS_NetType &GetType();

	CS_NetValue GetStaticField(const char *name, bool nonPublic = false);
	CS_NetValue GetField(const char *name, bool nonPublic =false);

	CS_NetValue GetStaticProperty(const char *name, bool nonPublic = false);
	CS_NetValue GetProperty(const char *name, bool nonPublic =false);
	bool SetStaticProperty(const char *name, const CS_NetValue &value, bool nonPublic = false);
	bool SetProperty(const char *name, const CS_NetValue &value, bool nonPublic =false);
};

class CPPSHARP_API CS_NetType : public CS_NetValue {
public:
	CS_NativeTypeData *typeData;

	static const CS_NetType &Type();
	static const CS_NetType &Integer();
	static const CS_NetType &String();
	static const CS_NetType &Char();
	static const CS_NetType &Float();
	static const CS_NetType &Double();
	static const CS_NetType &Boolean();

	static bool ParseFromName(const char *name, CS_NetType &_out);

	CS_NetType();
	CS_NetType(CS_NativeTypeData*);
	CS_NetType(const CS_NetType &other);
	CS_NetType(CS_NetType &&other);
	virtual ~CS_NetType();

	virtual CS_NetType &operator=(const CS_NetType &other);
	virtual CS_NetType &operator=(CS_NetType &&other);

	virtual CS_NetValue ToString();
};

CPPSHARP_API void CS_Init();
CPPSHARP_API bool CS_LoadReference(const char *path);
CPPSHARP_API void CS_LoadScripts(CS_Script *scripts, int len);

CPPSHARP_API void CS_RegisterFunction(
	const char *name, CS_NetValue (*func)(...),
	const CS_NetType &returnType,
	CS_NetType *paramTypes, int paramTypesLen);
CPPSHARP_API CS_NetValue CS_Invoke(const char *name, CS_NetValue *args, int argc);