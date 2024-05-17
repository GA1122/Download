static void voidMethodDictionaryArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodDictionaryArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Dictionary, dictionaryArg, Dictionary(info[0], info.GetIsolate()));
    if (!dictionaryArg.isUndefinedOrNull() && !dictionaryArg.isObject()) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodDictionaryArg", "TestObjectPython", "parameter 1 ('dictionaryArg') is not an object."), info.GetIsolate());
        return;
    }
    imp->voidMethodDictionaryArg(dictionaryArg);
}
