static void overloadedMethodC1Method(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("overloadedMethodC", "TestObject", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_VOID(Dictionary, dictionaryArg, Dictionary(info[0], info.GetIsolate()));
    if (!dictionaryArg.isUndefinedOrNull() && !dictionaryArg.isObject()) {
        throwTypeError(ExceptionMessages::failedToExecute("overloadedMethodC", "TestObject", "parameter 1 ('dictionaryArg') is not an object."), info.GetIsolate());
        return;
    }
    imp->overloadedMethodC(dictionaryArg);
}
