static void voidMethodSequenceStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodSequenceStringArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Vector<String>, sequenceStringArg, toNativeArray<String>(info[0], 1, info.GetIsolate()));
    imp->voidMethodSequenceStringArg(sequenceStringArg);
}
