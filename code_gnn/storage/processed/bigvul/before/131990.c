static void voidMethodVariadicStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Vector<String>, variadicStringArgs, toNativeArguments<String>(info, 0));
    imp->voidMethodVariadicStringArg(variadicStringArgs);
}
