static void voidMethodDefaultUndefinedTestInterfaceEmptyArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(TestInterfaceEmpty*, defaultUndefinedTestInterfaceEmptyArg, V8TestInterfaceEmpty::toNativeWithTypeCheck(info.GetIsolate(), info[0]));
    imp->voidMethodDefaultUndefinedTestInterfaceEmptyArg(defaultUndefinedTestInterfaceEmptyArg);
}
