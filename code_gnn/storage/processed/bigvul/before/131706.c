static void serializedScriptValueMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    v8SetReturnValue(info, imp->serializedScriptValueMethod() ? imp->serializedScriptValueMethod()->deserialize() : v8::Handle<v8::Value>(v8::Null(info.GetIsolate())));
}
