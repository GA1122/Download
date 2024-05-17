static void raisesExceptionTestInterfaceEmptyAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "raisesExceptionTestInterfaceEmptyAttribute", "TestObjectPython", info.Holder(), info.GetIsolate());
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(TestInterfaceEmpty*, cppValue, V8TestInterfaceEmpty::toNativeWithTypeCheck(info.GetIsolate(), jsValue));
    imp->setRaisesExceptionTestInterfaceEmptyAttribute(WTF::getPtr(cppValue), exceptionState);
    exceptionState.throwIfNeeded();
}
