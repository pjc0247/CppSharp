#include "stdafx.h"

#include "CppSharpBridge.h"

CS_NetValue my_sum(CS_NetValue a, CS_NetValue b) {
	printf("sum %d %d", a._v.i, b._v.i);

	return CS_NetValue::Integer(
		a._v.i + b._v.i);
}

#pragma managed
int main()
{
	CS_NetType *paramTypes = new CS_NetType[2];
	paramTypes[0] = CS_NetType::Integer();
	paramTypes[1] = CS_NetType::Integer();

	CS_RegisterFunction("sum", (CS_NetValue(*)(...))my_sum,
		CS_NetType::Integer(), paramTypes, 2);

	CS_Script *scripts = new CS_Script[1];
	scripts[0].path = "ASDF";
	scripts[0].source = "using System; public string AA(int a) { var s = sum(1,2); Console.WriteLine(s); return \"aa\"; }";

	CS_LoadScripts(scripts, 1);

	CS_NetValue *args = new CS_NetValue[1];
	args[0].type = CS_Integer;
	args[0]._v.i = 1000;

	auto ret = CS_Invoke("AA", args, 1);

	printf("ret %s\n", ret._v.s);

    return 0;
}

