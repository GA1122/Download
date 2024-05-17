static void treatReturnedNullStringAsUndefinedStringMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    v8SetReturnValueStringOrUndefined(info, imp->treatReturnedNullStringAsUndefinedStringMethod(), info.GetIsolate());
}
