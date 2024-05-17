static void voidMethodOptionalLongArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "voidMethodOptionalLongArg", "TestObjectPython", info.Holder(), info.GetIsolate());
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    if (UNLIKELY(info.Length() <= 0)) {
        imp->voidMethodOptionalLongArg();
        return;
    }
    V8TRYCATCH_EXCEPTION_VOID(int, optionalLongArg, toInt32(info[0], exceptionState), exceptionState);
    imp->voidMethodOptionalLongArg(optionalLongArg);
}
