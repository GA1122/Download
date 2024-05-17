static void voidMethodSerializedScriptValueArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "voidMethodSerializedScriptValueArg", "TestObjectPython", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    RefPtr<SerializedScriptValue> serializedScriptValueArg = SerializedScriptValue::create(info[0], 0, 0, exceptionState, info.GetIsolate());
    if (exceptionState.throwIfNeeded())
        return;
    imp->voidMethodSerializedScriptValueArg(serializedScriptValueArg);
}
