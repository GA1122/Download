static void stringAttrWithGetterExceptionAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    ExceptionState exceptionState(ExceptionState::GetterContext, "stringAttrWithGetterException", "TestObject", info.Holder(), info.GetIsolate());
    String jsValue = imp->stringAttrWithGetterException(exceptionState);
    if (UNLIKELY(exceptionState.throwIfNeeded()))
        return;
    v8SetReturnValueString(info, jsValue, info.GetIsolate());
}
