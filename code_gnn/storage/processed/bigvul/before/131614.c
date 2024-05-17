static void raisesExceptionLongAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    ExceptionState exceptionState(ExceptionState::GetterContext, "raisesExceptionLongAttribute", "TestObjectPython", info.Holder(), info.GetIsolate());
    int jsValue = imp->raisesExceptionLongAttribute(exceptionState);
    if (UNLIKELY(exceptionState.throwIfNeeded()))
        return;
    v8SetReturnValueInt(info, jsValue);
}