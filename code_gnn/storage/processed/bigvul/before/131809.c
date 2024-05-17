static void treatReturnedNullStringAsNullStringMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    v8SetReturnValueStringOrNull(info, imp->treatReturnedNullStringAsNullStringMethod(), info.GetIsolate());
}
