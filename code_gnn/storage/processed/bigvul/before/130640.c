static void attrWithGetterExceptionAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    ExceptionState exceptionState(ExceptionState::GetterContext, "attrWithGetterException", "TestObject", info.Holder(), info.GetIsolate());
    int jsValue = imp->attrWithGetterException(exceptionState);
    if (UNLIKELY(exceptionState.throwIfNeeded()))
        return;
    v8SetReturnValueInt(info, jsValue);
}
