static void overloadedMethodCMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (((info.Length() == 1) && (info[0]->IsObject()))) {
        overloadedMethodC1Method(info);
        return;
    }
    if (((info.Length() == 1))) {
        overloadedMethodC2Method(info);
        return;
    }
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "overloadedMethodC", "TestObject", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    exceptionState.throwTypeError("No function was found that matched the signature provided.");
    exceptionState.throwIfNeeded();
}
