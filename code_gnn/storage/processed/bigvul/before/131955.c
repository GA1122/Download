static void voidMethodSequenceTestInterfaceEmptyArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodSequenceTestInterfaceEmptyArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Vector<RefPtr<TestInterfaceEmpty> >, sequenceTestInterfaceEmptyArg, (toRefPtrNativeArray<TestInterfaceEmpty, V8TestInterfaceEmpty>(info[0], 1, info.GetIsolate())));
    imp->voidMethodSequenceTestInterfaceEmptyArg(sequenceTestInterfaceEmptyArg);
}
