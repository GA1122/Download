static void methodWithEnforceRangeUInt64Method(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "methodWithEnforceRangeUInt64", "TestObject", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(unsigned long long, value, toUInt64(info[0], EnforceRange, exceptionState), exceptionState);
    imp->methodWithEnforceRangeUInt64(value);
}
