static void methodThatRequiresAllArgsAndThrowsMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "methodThatRequiresAllArgsAndThrows", "TestObject", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 2)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(2, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, strArg, info[0]);
    V8TRYCATCH_VOID(TestObject*, objArg, V8TestObject::toNativeWithTypeCheck(info.GetIsolate(), info[1]));
    RefPtr<TestObject> result = imp->methodThatRequiresAllArgsAndThrows(strArg, objArg, exceptionState);
    if (exceptionState.throwIfNeeded())
        return;
    v8SetReturnValue(info, result.release());
}
