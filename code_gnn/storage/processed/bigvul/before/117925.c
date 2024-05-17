static v8::Handle<v8::Value> cachedValueAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestSerializedScriptValueInterface.cachedValue._get");
    v8::Handle<v8::String> propertyName = v8::String::NewSymbol("cachedValue");
    v8::Handle<v8::Value> value = info.Holder()->GetHiddenValue(propertyName);
    if (!value.IsEmpty())
        return value;
    TestSerializedScriptValueInterface* imp = V8TestSerializedScriptValueInterface::toNative(info.Holder());
    SerializedScriptValue* serialized = imp->cachedValue();
    value = serialized ? serialized->deserialize() : v8::Handle<v8::Value>(v8::Null());
    info.Holder()->SetHiddenValue(propertyName, value);
    return value;
}
