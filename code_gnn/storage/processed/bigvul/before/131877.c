static void voidMethodClampUnsignedLongArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "voidMethodClampUnsignedLongArg", "TestObjectPython", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    unsigned clampUnsignedLongArg = 0;
    V8TRYCATCH_VOID(double, clampUnsignedLongArgNativeValue, info[0]->NumberValue());
    if (!std::isnan(clampUnsignedLongArgNativeValue))
        clampUnsignedLongArg = clampTo<unsigned long>(clampUnsignedLongArgNativeValue);
    imp->voidMethodClampUnsignedLongArg(clampUnsignedLongArg);
}
