static void overloadedMethodA2Method(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "overloadedMethodA", "TestObjectPython", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 2)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(2, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(int, longArg1, toInt32(info[0], exceptionState), exceptionState);
    V8TRYCATCH_EXCEPTION_VOID(int, longArg2, toInt32(info[1], exceptionState), exceptionState);
    imp->overloadedMethodA(longArg1, longArg2);
}
