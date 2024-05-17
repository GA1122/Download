static void voidMethodStringArgLongArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "voidMethodStringArgLongArg", "TestObjectPython", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 2)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(2, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, stringArg, info[0]);
    V8TRYCATCH_EXCEPTION_VOID(int, longArg, toInt32(info[1], exceptionState), exceptionState);
    imp->voidMethodStringArgLongArg(stringArg, longArg);
}