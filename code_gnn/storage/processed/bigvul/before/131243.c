static void cachedAttributeAnyAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Handle<v8::String> propertyName = v8AtomicString(info.GetIsolate(), "cachedAttributeAnyAttribute");
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    if (!imp->isValueDirty()) {
        v8::Handle<v8::Value> jsValue = V8HiddenValue::getHiddenValue(info.GetIsolate(), info.Holder(), propertyName);
        if (!jsValue.IsEmpty()) {
            v8SetReturnValue(info, jsValue);
            return;
        }
    }
    ScriptValue jsValue = imp->cachedAttributeAnyAttribute();
    V8HiddenValue::setHiddenValue(info.GetIsolate(), info.Holder(), propertyName, jsValue.v8Value());
    v8SetReturnValue(info, jsValue.v8Value());
}
