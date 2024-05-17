static v8::Handle<v8::Value> readonlyValueAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestSerializedScriptValueInterface.readonlyValue._get");
    TestSerializedScriptValueInterface* imp = V8TestSerializedScriptValueInterface::toNative(info.Holder());
    return imp->readonlyValue() ? imp->readonlyValue()->deserialize() : v8::Handle<v8::Value>(v8::Null());
}
