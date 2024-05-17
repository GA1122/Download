void setJSTestSerializedScriptValueInterfaceValue(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestSerializedScriptValueInterface* castedThis = jsCast<JSTestSerializedScriptValueInterface*>(thisObject);
    TestSerializedScriptValueInterface* impl = static_cast<TestSerializedScriptValueInterface*>(castedThis->impl());
    impl->setValue(SerializedScriptValue::create(exec, value));
}
