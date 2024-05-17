static void raisesExceptionTestInterfaceEmptyAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    ExceptionState exceptionState(ExceptionState::GetterContext, "raisesExceptionTestInterfaceEmptyAttribute", "TestObjectPython", info.Holder(), info.GetIsolate());
    RefPtr<TestInterfaceEmpty> jsValue = imp->raisesExceptionTestInterfaceEmptyAttribute(exceptionState);
    if (UNLIKELY(exceptionState.throwIfNeeded()))
        return;
    v8SetReturnValueFast(info, WTF::getPtr(jsValue.release()), imp);
}
