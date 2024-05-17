static void methodWithExceptionMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "methodWithException", "TestObject", info.Holder(), info.GetIsolate());
    TestObject* imp = V8TestObject::toNative(info.Holder());
    imp->methodWithException(exceptionState);
    if (exceptionState.throwIfNeeded())
        return;
}
