static void cachedAttributeRaisesExceptionGetterAnyAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "cachedAttributeRaisesExceptionGetterAnyAttribute", "TestObjectPython", info.Holder(), info.GetIsolate());
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(ScriptValue, cppValue, ScriptValue(jsValue, info.GetIsolate()));
    imp->setCachedAttributeRaisesExceptionGetterAnyAttribute(cppValue, exceptionState);
    exceptionState.throwIfNeeded();
    V8HiddenValue::deleteHiddenValue(info.GetIsolate(), info.Holder(), v8AtomicString(info.GetIsolate(), "cachedAttributeRaisesExceptionGetterAnyAttribute"));  
}
