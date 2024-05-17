static void cachedAttributeRaisesExceptionGetterAnyAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Handle<v8::String> propertyName = v8AtomicString(info.GetIsolate(), "cachedAttributeRaisesExceptionGetterAnyAttribute");
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    if (!imp->isValueDirty()) {
        v8::Handle<v8::Value> jsValue = V8HiddenValue::getHiddenValue(info.GetIsolate(), info.Holder(), propertyName);
        if (!jsValue.IsEmpty()) {
            v8SetReturnValue(info, jsValue);
            return;
        }
    }
    ExceptionState exceptionState(ExceptionState::GetterContext, "cachedAttributeRaisesExceptionGetterAnyAttribute", "TestObjectPython", info.Holder(), info.GetIsolate());
    ScriptValue jsValue = imp->cachedAttributeRaisesExceptionGetterAnyAttribute(exceptionState);
    if (UNLIKELY(exceptionState.throwIfNeeded()))
        return;
    V8HiddenValue::setHiddenValue(info.GetIsolate(), info.Holder(), propertyName, jsValue.v8Value());
    v8SetReturnValue(info, jsValue.v8Value());
}