static void methodWithEnumArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("methodWithEnumArg", "TestObject", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, enumArg, info[0]);
    String string = enumArg;
    if (!(string == "" || string == "EnumValue1" || string == "EnumValue2" || string == "EnumValue3")) {
        throwTypeError(ExceptionMessages::failedToExecute("methodWithEnumArg", "TestObject", "parameter 1 ('" + string + "') is not a valid enum value."), info.GetIsolate());
        return;
    }
    imp->methodWithEnumArg(enumArg);
}
