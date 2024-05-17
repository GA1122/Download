void setJSTestSerializedScriptValueInterfaceCachedValue(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestSerializedScriptValueInterface* castedThis = jsCast<JSTestSerializedScriptValueInterface*>(thisObject);
    TestSerializedScriptValueInterface* impl = static_cast<TestSerializedScriptValueInterface*>(castedThis->impl());
    impl->setCachedValue(SerializedScriptValue::create(exec, value));
}
