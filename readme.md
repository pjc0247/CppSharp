CppSharp
====

Usage
----
__load scripts__
```cpp
CS_LoadScripts(scripts, 1);
```

```cpp
CS_NetValue my_sum(CS_NetValue a, CS_NetValue b) {
  return CS_NetValue::Integer(a.Int() + b.Int());
}

CS_RegisterFunction("Sum", (CS_Function)my_sum);
```

```cpp
CS_NetValue *args = new CS_NetValue[2];
args[0] = CS_NetValue::Integer(1);
args[1] = CS_NetValue::Integer(5);

CS_Invoke("SetUpUsBomb", args, 1);
```

```cpp
CS_NetType dateTimeType;
if (CS_NetType::ParseFromName("System.DateTime", dateTimeType)) {
  auto now = dateTimeType.GetStaticProperty("Now");
  printf("%s \n", now.ToString().Cstr());
}
```
