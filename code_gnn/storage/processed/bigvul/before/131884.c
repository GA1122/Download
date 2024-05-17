static void voidMethodDefaultNullStringStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, defaultNullStringStringArg, argumentOrNull(info, 0));
    imp->voidMethodDefaultNullStringStringArg(defaultNullStringStringArg);
}
