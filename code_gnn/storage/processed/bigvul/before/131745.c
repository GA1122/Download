static void strictTypeCheckingTestInterfaceAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "strictTypeCheckingTestInterfaceAttribute", "TestObjectPython", info.Holder(), info.GetIsolate());
    if (!isUndefinedOrNull(jsValue) && !V8TestInterface::hasInstance(jsValue, info.GetIsolate())) {
        exceptionState.throwTypeError("The provided value is not of type 'TestInterface'.");
        exceptionState.throwIfNeeded();
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(TestInterface*, cppValue, V8TestInterface::toNativeWithTypeCheck(info.GetIsolate(), jsValue));
    imp->setStrictTypeCheckingTestInterfaceAttribute(WTF::getPtr(cppValue));
}
