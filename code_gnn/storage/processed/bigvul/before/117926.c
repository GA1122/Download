static void cachedValueAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestSerializedScriptValueInterface.cachedValue._set");
    TestSerializedScriptValueInterface* imp = V8TestSerializedScriptValueInterface::toNative(info.Holder());
    RefPtr<SerializedScriptValue> v = SerializedScriptValue::create(value, info.GetIsolate());
    imp->setCachedValue(WTF::getPtr(v));
    info.Holder()->DeleteHiddenValue(v8::String::NewSymbol("cachedValue"));  
    return;
}
