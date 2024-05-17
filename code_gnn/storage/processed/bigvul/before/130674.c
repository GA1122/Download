static void classMethodWithClampMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "classMethodWithClamp", "TestObject", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 2)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(2, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    unsigned objArgsShort = 0;
    V8TRYCATCH_VOID(double, objArgsShortNativeValue, info[0]->NumberValue());
    if (!std::isnan(objArgsShortNativeValue))
        objArgsShort = clampTo<unsigned short>(objArgsShortNativeValue);
    unsigned objArgsLong = 0;
    V8TRYCATCH_VOID(double, objArgsLongNativeValue, info[1]->NumberValue());
    if (!std::isnan(objArgsLongNativeValue))
        objArgsLong = clampTo<unsigned long>(objArgsLongNativeValue);
    imp->classMethodWithClamp(objArgsShort, objArgsLong);
}
