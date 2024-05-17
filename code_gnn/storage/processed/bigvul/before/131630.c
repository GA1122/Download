static void raisesExceptionVoidMethodOptionalTestCallbackInterfaceArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "raisesExceptionVoidMethodOptionalTestCallbackInterfaceArg", "TestObjectPython", info.Holder(), info.GetIsolate());
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    OwnPtr<TestCallbackInterface> optionalTestCallbackInterfaceArg;
    if (info.Length() > 0 && !isUndefinedOrNull(info[0])) {
        if (!info[0]->IsFunction()) {
            exceptionState.throwTypeError("The callback provided as parameter 1 is not a function.");
            exceptionState.throwIfNeeded();
            return;
        }
        optionalTestCallbackInterfaceArg = V8TestCallbackInterface::create(v8::Handle<v8::Function>::Cast(info[0]), currentExecutionContext(info.GetIsolate()));
    }
    imp->raisesExceptionVoidMethodOptionalTestCallbackInterfaceArg(optionalTestCallbackInterfaceArg.release(), exceptionState);
    if (exceptionState.throwIfNeeded())
        return;
}
