static void unsignedLongLongMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    v8SetReturnValue(info, static_cast<double>(imp->unsignedLongLongMethod()));
}
