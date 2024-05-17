static void testInterfaceEmptyArrayMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    v8SetReturnValue(info, v8Array(imp->testInterfaceEmptyArrayMethod(), info.GetIsolate()));
}
