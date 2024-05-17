static void voidMethodSequenceDictionaryArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodSequenceDictionaryArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Vector<Dictionary>, sequenceDictionaryArg, toNativeArray<Dictionary>(info[0], 1, info.GetIsolate()));
    imp->voidMethodSequenceDictionaryArg(sequenceDictionaryArg);
}
