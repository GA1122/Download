static void serializedScriptValueAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    v8SetReturnValue(info, imp->serializedScriptValueAttribute() ? imp->serializedScriptValueAttribute()->deserialize() : v8::Handle<v8::Value>(v8::Null(info.GetIsolate())));
}
