static void serializedValueMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "serializedValue", "TestObject", info.Holder(), info.GetIsolate());
    if (UNLIKELY(info.Length() < 1)) {
        exceptionState.throwTypeError(ExceptionMessages::notEnoughArguments(1, info.Length()));
        exceptionState.throwIfNeeded();
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    RefPtr<SerializedScriptValue> serializedArg = SerializedScriptValue::create(info[0], 0, 0, exceptionState, info.GetIsolate());
    if (exceptionState.throwIfNeeded())
        return;
    imp->serializedValue(serializedArg);
}
