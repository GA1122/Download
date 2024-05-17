static void voidMethodTestEnumArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodTestEnumArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, testEnumTypeArg, info[0]);
    String string = testEnumTypeArg;
    if (!(string == "" || string == "EnumValue1" || string == "EnumValue2" || string == "EnumValue3")) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodTestEnumArg", "TestObjectPython", "parameter 1 ('" + string + "') is not a valid enum value."), info.GetIsolate());
        return;
    }
    imp->voidMethodTestEnumArg(testEnumTypeArg);
}
