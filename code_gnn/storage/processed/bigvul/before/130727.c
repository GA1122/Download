static void domStringListFunctionMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "domStringListFunction", "TestObject", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_VOID(DOMStringList*, values, V8DOMStringList::toNativeWithTypeCheck(info.GetIsolate(), info[0]));
    RefPtr<DOMStringList> result = imp->domStringListFunction(values, exceptionState);
    if (exceptionState.throwIfNeeded())
        return;
    v8SetReturnValue(info, result.release());
}
