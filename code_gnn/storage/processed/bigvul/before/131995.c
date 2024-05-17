static void voidMethodVoidCallbackFunctionArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodVoidCallbackFunctionArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(ScriptValue, voidCallbackFunctionArg, ScriptValue(info[0], info.GetIsolate()));
    imp->voidMethodVoidCallbackFunctionArg(voidCallbackFunctionArg);
}