static void methodWithNonOptionalArgAndOptionalArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "methodWithNonOptionalArgAndOptionalArg", "TestObject", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(int, nonOpt, toInt32(info[0], exceptionState), exceptionState);
    if (UNLIKELY(info.Length() <= 1)) {
        imp->methodWithNonOptionalArgAndOptionalArg(nonOpt);
        return;
    }
    V8TRYCATCH_EXCEPTION_VOID(int, opt, toInt32(info[1], exceptionState), exceptionState);
    imp->methodWithNonOptionalArgAndOptionalArg(nonOpt, opt);
}
