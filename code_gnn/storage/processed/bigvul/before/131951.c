static void voidMethodSequenceLongArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodSequenceLongArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Vector<int>, sequenceLongArg, toNativeArray<int>(info[0], 1, info.GetIsolate()));
    imp->voidMethodSequenceLongArg(sequenceLongArg);
}
