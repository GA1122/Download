static void voidMethodDefaultUndefinedStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, defaultUndefinedStringArg, info[0]);
    imp->voidMethodDefaultUndefinedStringArg(defaultUndefinedStringArg);
}
