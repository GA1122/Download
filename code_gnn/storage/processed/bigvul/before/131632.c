static void raisesExceptionVoidMethodTestCallbackInterfaceArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "raisesExceptionVoidMethodTestCallbackInterfaceArg", "TestObjectPython", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    if (info.Length() <= 0 || !info[0]->IsFunction()) {
        exceptionState.throwTypeError("The callback provided as parameter 1 is not a function.");
        exceptionState.throwIfNeeded();
        return;
    }
    OwnPtr<TestCallbackInterface> testCallbackInterfaceArg = V8TestCallbackInterface::create(v8::Handle<v8::Function>::Cast(info[0]), currentExecutionContext(info.GetIsolate()));
    imp->raisesExceptionVoidMethodTestCallbackInterfaceArg(testCallbackInterfaceArg.release(), exceptionState);
    if (exceptionState.throwIfNeeded())
        return;
}
