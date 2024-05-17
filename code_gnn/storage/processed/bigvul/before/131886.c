static void voidMethodDefaultUndefinedLongArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "voidMethodDefaultUndefinedLongArg", "TestObjectPython", info.Holder(), info.GetIsolate());
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(int, defaultUndefinedLongArg, toInt32(info[0], exceptionState), exceptionState);
    imp->voidMethodDefaultUndefinedLongArg(defaultUndefinedLongArg);
}
