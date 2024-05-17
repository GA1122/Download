static void overloadedMethodG1Method(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    imp->overloadedMethodG();
}
