static void overloadedMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (((info.Length() == 1))) {
        overloadedMethod1Method(info);
        return;
    }
    if (((info.Length() == 1) && (info[0]->IsNull() || info[0]->IsFunction()))) {
        overloadedMethod2Method(info);
        return;
    }
    if (((info.Length() == 1) && (V8TestObject::hasInstance(info[0], info.GetIsolate())))) {
        overloadedMethod3Method(info);
        return;
    }
    if (((info.Length() == 1) && (info[0]->IsArray()))) {
        overloadedMethod4Method(info);
        return;
    }
    if (((info.Length() == 1) && (info[0]->IsArray()))) {
        overloadedMethod5Method(info);
        return;
    }
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "overloadedMethod", "TestObject", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    exceptionState.throwTypeError("No function was found that matched the signature provided.");
    exceptionState.throwIfNeeded();
}
