static void overloadedMethodA1Method(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "overloadedMethodA", "TestObject", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_VOID(TestObject*, objArg, V8TestObject::toNativeWithTypeCheck(info.GetIsolate(), info[0]));
    if (UNLIKELY(info.Length() <= 1)) {
        imp->overloadedMethodA(objArg);
        return;
    }
    V8TRYCATCH_EXCEPTION_VOID(int, longArg, toInt32(info[1], exceptionState), exceptionState);
    imp->overloadedMethodA(objArg, longArg);
}
