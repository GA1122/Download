static void voidMethodNullableStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodNullableStringArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(TestInterfaceEmpty*, nullableTestInterfaceEmptyArg, V8TestInterfaceEmpty::toNativeWithTypeCheck(info.GetIsolate(), info[0]));
    imp->voidMethodNullableStringArg(nullableTestInterfaceEmptyArg);
}