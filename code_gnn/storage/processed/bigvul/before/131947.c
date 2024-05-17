static void voidMethodPromiseArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodPromiseArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(ScriptPromise, promiseArg, ScriptPromise(info[0], info.GetIsolate()));
    if (!promiseArg.isUndefinedOrNull() && !promiseArg.isObject()) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodPromiseArg", "TestObjectPython", "parameter 1 ('promiseArg') is not an object."), info.GetIsolate());
        return;
    }
    imp->voidMethodPromiseArg(promiseArg);
}
