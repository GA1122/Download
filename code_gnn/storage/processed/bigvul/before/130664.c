static void callbackFunctionArgumentMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("callbackFunctionArgument", "TestObject", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_VOID(ScriptValue, function, ScriptValue(info[0], info.GetIsolate()));
    imp->callbackFunctionArgument(function);
}
