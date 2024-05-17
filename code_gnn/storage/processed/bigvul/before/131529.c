static void overloadedMethodBMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (((info.Length() == 1))) {
        overloadedMethodB1Method(info);
        return;
    }
    if (((info.Length() == 1)) || ((info.Length() == 2))) {
        overloadedMethodB2Method(info);
        return;
    }
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "overloadedMethodB", "TestObjectPython", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    exceptionState.throwTypeError("No function was found that matched the signature provided.");
    exceptionState.throwIfNeeded();
}
