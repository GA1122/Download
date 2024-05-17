static void voidMethodOptionalDictionaryArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Dictionary, optionalDictionaryArg, Dictionary(info[0], info.GetIsolate()));
    if (!optionalDictionaryArg.isUndefinedOrNull() && !optionalDictionaryArg.isObject()) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodOptionalDictionaryArg", "TestObjectPython", "parameter 1 ('optionalDictionaryArg') is not an object."), info.GetIsolate());
        return;
    }
    imp->voidMethodOptionalDictionaryArg(optionalDictionaryArg);
}
