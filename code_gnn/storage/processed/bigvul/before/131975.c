static void voidMethodTestInterfaceEmptyArgOptionalLongArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "voidMethodTestInterfaceEmptyArgOptionalLongArg", "TestObjectPython", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(TestInterfaceEmpty*, optionalTestInterfaceEmpty, V8TestInterfaceEmpty::toNativeWithTypeCheck(info.GetIsolate(), info[0]));
    if (UNLIKELY(info.Length() <= 1)) {
        imp->voidMethodTestInterfaceEmptyArgOptionalLongArg(optionalTestInterfaceEmpty);
        return;
    }
    V8TRYCATCH_EXCEPTION_VOID(int, longArg, toInt32(info[1], exceptionState), exceptionState);
    imp->voidMethodTestInterfaceEmptyArgOptionalLongArg(optionalTestInterfaceEmpty, longArg);
}
