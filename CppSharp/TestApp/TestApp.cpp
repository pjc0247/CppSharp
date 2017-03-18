#include "stdafx.h"

#include "CppSharpBridge.h"

CS_NetValue my_sum(CS_NetValue *args, int argc) {
	printf("sum");

	return CS_NetValue::Integer(
		_AS_INT(args, 0) +_AS_INT(args, 1));
}

int main()
{
	CS_Script *scripts = new CS_Script[1];
	scripts[0].path = "ASDF";
	scripts[0].source = "using System; public string AA(int a) { CSFunctionBindings.Invoke(\"sum\", null); Console.WriteLine(a); return \"aa\"; }";

	CS_LoadScripts(scripts, 1);

	CS_NetValue *args = new CS_NetValue[1];
	args[0].type = CS_Integer;
	args[0].i = 1000;

	CS_RegisterFunction("sum", my_sum);

	auto ret = CS_Invoke("AA", args, 1);

	printf("%s\n", ret.s);

    return 0;
}

