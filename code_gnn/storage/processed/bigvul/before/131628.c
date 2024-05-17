static void raisesExceptionVoidMethodOptionalLongArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "raisesExceptionVoidMethodOptionalLongArg", "TestObjectPython", info.Holder(), info.GetIsolate());
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    if (UNLIKELY(info.Length() <= 0)) {
        imp->raisesExceptionVoidMethodOptionalLongArg(exceptionState);
        if (exceptionState.throwIfNeeded())
            return;
        return;
    }
    V8TRYCATCH_EXCEPTION_VOID(int, optionalLongArg, toInt32(info[0], exceptionState), exceptionState);
    imp->raisesExceptionVoidMethodOptionalLongArg(optionalLongArg, exceptionState);
    if (exceptionState.throwIfNeeded())
        return;
}
