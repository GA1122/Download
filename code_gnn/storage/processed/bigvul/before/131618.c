static void raisesExceptionStringMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "raisesExceptionStringMethod", "TestObjectPython", info.Holder(), info.GetIsolate());
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    String result = imp->raisesExceptionStringMethod(exceptionState);
    if (exceptionState.throwIfNeeded())
        return;
    v8SetReturnValueString(info, result, info.GetIsolate());
}
