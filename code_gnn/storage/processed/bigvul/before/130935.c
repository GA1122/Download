static void overloadedMethodAMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (((info.Length() == 1) && (info[0]->IsNull() || V8TestObject::hasInstance(info[0], info.GetIsolate()))) || ((info.Length() == 2) && (info[0]->IsNull() || V8TestObject::hasInstance(info[0], info.GetIsolate())))) {
        overloadedMethodA1Method(info);
        return;
    }
    if (((info.Length() == 2) && (info[0]->IsNull() || V8TestObject::hasInstance(info[0], info.GetIsolate())))) {
        overloadedMethodA2Method(info);
        return;
    }
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "overloadedMethodA", "TestObject", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    exceptionState.throwTypeError("No function was found that matched the signature provided.");
    exceptionState.throwIfNeeded();
}
