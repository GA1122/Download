static void raisesExceptionTestInterfaceEmptyVoidMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "raisesExceptionTestInterfaceEmptyVoidMethod", "TestObjectPython", info.Holder(), info.GetIsolate());
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    RefPtr<TestInterfaceEmpty> result = imp->raisesExceptionTestInterfaceEmptyVoidMethod(exceptionState);
    if (exceptionState.throwIfNeeded())
        return;
    v8SetReturnValue(info, result.release());
}
