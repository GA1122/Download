static void methodWithSequenceArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("methodWithSequenceArg", "TestObject", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_VOID(Vector<RefPtr<TestInterface> >, sequenceArg, (toRefPtrNativeArray<TestInterface, V8TestInterface>(info[0], 1, info.GetIsolate())));
    imp->methodWithSequenceArg(sequenceArg);
}
