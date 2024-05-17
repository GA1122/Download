static void strictTypeCheckingVoidMethodNullableTestInterfaceEmptyArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("strictTypeCheckingVoidMethodNullableTestInterfaceEmptyArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    if (info.Length() > 0 && !isUndefinedOrNull(info[0]) && !V8TestInterfaceEmpty::hasInstance(info[0], info.GetIsolate())) {
        throwTypeError(ExceptionMessages::failedToExecute("strictTypeCheckingVoidMethodNullableTestInterfaceEmptyArg", "TestObjectPython", "parameter 1 is not of type 'TestInterfaceEmpty'."), info.GetIsolate());
        return;
    }
    V8TRYCATCH_VOID(TestInterfaceEmpty*, testInterfaceEmptyArg, V8TestInterfaceEmpty::toNativeWithTypeCheck(info.GetIsolate(), info[0]));
    imp->strictTypeCheckingVoidMethodNullableTestInterfaceEmptyArg(testInterfaceEmptyArg);
}
