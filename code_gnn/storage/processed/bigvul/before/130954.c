static void overloadedPerWorldMethodMethodForMainWorld(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (((info.Length() == 1))) {
        overloadedPerWorldMethod1MethodForMainWorld(info);
        return;
    }
    if (((info.Length() == 2))) {
        overloadedPerWorldMethod2MethodForMainWorld(info);
        return;
    }
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "overloadedPerWorldMethod", "TestObject", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    exceptionState.throwTypeError("No function was found that matched the signature provided.");
    exceptionState.throwIfNeeded();
}
