CppSharp
====

Usage
----
__load scripts__
```cpp
CS_LoadScripts(scripts, 1);
```

```cpp
CS_NetValue my_sum(CS_NetValue *args, int argc) {
	return CS_NetValue::Integer(
		_AS_INT(args, 0) +_AS_INT(args, 1));
}

CS_RegisterFunction("Sum", my_sum);
```

```cpp
CS_NetValue *args = new CS_NetValue[2];
args[0] = CS_NetValue::Integer(1);
args[1] = CS_NetValue::Integer(5);

CS_Invoke("SetUpUsBomb", args, 1);
```