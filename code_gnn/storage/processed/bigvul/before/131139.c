static void withScriptStateAttributeRaisesAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    ExceptionState exceptionState(ExceptionState::GetterContext, "withScriptStateAttributeRaises", "TestObject", info.Holder(), info.GetIsolate());
    RefPtr<TestObject> jsValue = imp->withScriptStateAttributeRaises(exceptionState);
    if (UNLIKELY(exceptionState.throwIfNeeded()))
        return;
    v8SetReturnValueFast(info, WTF::getPtr(jsValue.release()), imp);
}
