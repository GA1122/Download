static void methodWithNonOptionalArgAndTwoOptionalArgsMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "methodWithNonOptionalArgAndTwoOptionalArgs", "TestObject", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(int, nonOpt, toInt32(info[0], exceptionState), exceptionState);
    if (UNLIKELY(info.Length() <= 1)) {
        imp->methodWithNonOptionalArgAndTwoOptionalArgs(nonOpt);
        return;
    }
    V8TRYCATCH_EXCEPTION_VOID(int, opt1, toInt32(info[1], exceptionState), exceptionState);
    if (UNLIKELY(info.Length() <= 2)) {
        imp->methodWithNonOptionalArgAndTwoOptionalArgs(nonOpt, opt1);
        return;
    }
    V8TRYCATCH_EXCEPTION_VOID(int, opt2, toInt32(info[2], exceptionState), exceptionState);
    imp->methodWithNonOptionalArgAndTwoOptionalArgs(nonOpt, opt1, opt2);
}
