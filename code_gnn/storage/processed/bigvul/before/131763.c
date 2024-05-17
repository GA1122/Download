static void stringMethodOptionalLongArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "stringMethodOptionalLongArg", "TestObjectPython", info.Holder(), info.GetIsolate());
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    if (UNLIKELY(info.Length() <= 0)) {
        v8SetReturnValueString(info, imp->stringMethodOptionalLongArg(), info.GetIsolate());
        return;
    }
    V8TRYCATCH_EXCEPTION_VOID(int, optionalLongArg, toInt32(info[0], exceptionState), exceptionState);
    v8SetReturnValueString(info, imp->stringMethodOptionalLongArg(optionalLongArg), info.GetIsolate());
}
