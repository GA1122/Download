static void perWorldBindingsRuntimeEnabledVoidMethodMethodForMainWorld(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    imp->perWorldBindingsRuntimeEnabledVoidMethod();
}