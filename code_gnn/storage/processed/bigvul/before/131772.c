static void testEnumMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    v8SetReturnValueString(info, imp->testEnumMethod(), info.GetIsolate());
}
