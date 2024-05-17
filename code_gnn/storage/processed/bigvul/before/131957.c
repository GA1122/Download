static void voidMethodSequenceTestInterfaceWillBeGarbageCollectedArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodSequenceTestInterfaceWillBeGarbageCollectedArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(WillBeHeapVector<RefPtrWillBeMember<TestInterfaceWillBeGarbageCollected> >, sequenceTestInterfaceWillBeGarbageCollectedArg, (toMemberNativeArray<TestInterfaceWillBeGarbageCollected, V8TestInterfaceWillBeGarbageCollected>(info[0], 1, info.GetIsolate())));
    imp->voidMethodSequenceTestInterfaceWillBeGarbageCollectedArg(sequenceTestInterfaceWillBeGarbageCollectedArg);
}
