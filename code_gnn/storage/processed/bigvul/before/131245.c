static void cachedAttributeAnyAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(ScriptValue, cppValue, ScriptValue(jsValue, info.GetIsolate()));
    imp->setCachedAttributeAnyAttribute(cppValue);
    V8HiddenValue::deleteHiddenValue(info.GetIsolate(), info.Holder(), v8AtomicString(info.GetIsolate(), "cachedAttributeAnyAttribute"));  
}
