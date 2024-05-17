static void overloadedPerWorldMethod2MethodForMainWorld(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "overloadedPerWorldMethod", "TestObject", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 2)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(2, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, strArg, info[0]);
    V8TRYCATCH_EXCEPTION_VOID(int, longArg, toInt32(info[1], exceptionState), exceptionState);
    imp->overloadedPerWorldMethod(strArg, longArg);
}
