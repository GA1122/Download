static void overloadedMethodB2Method(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "overloadedMethodB", "TestObjectPython", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(int, longArg1, toInt32(info[0], exceptionState), exceptionState);
    if (UNLIKELY(info.Length() <= 1)) {
        imp->overloadedMethodB(longArg1);
        return;
    }
    V8TRYCATCH_EXCEPTION_VOID(int, longArg2, toInt32(info[1], exceptionState), exceptionState);
    imp->overloadedMethodB(longArg1, longArg2);
}
