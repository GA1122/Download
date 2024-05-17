static void voidMethodClampUnsignedShortArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "voidMethodClampUnsignedShortArg", "TestObjectPython", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    unsigned clampUnsignedShortArg = 0;
    V8TRYCATCH_VOID(double, clampUnsignedShortArgNativeValue, info[0]->NumberValue());
    if (!std::isnan(clampUnsignedShortArgNativeValue))
        clampUnsignedShortArg = clampTo<unsigned short>(clampUnsignedShortArgNativeValue);
    imp->voidMethodClampUnsignedShortArg(clampUnsignedShortArg);
}
