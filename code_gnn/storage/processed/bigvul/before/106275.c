JSValue jsTestSerializedScriptValueInterfaceValue(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestSerializedScriptValueInterface* castedThis = jsCast<JSTestSerializedScriptValueInterface*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    TestSerializedScriptValueInterface* impl = static_cast<TestSerializedScriptValueInterface*>(castedThis->impl());
    JSValue result = impl->value() ? impl->value()->deserialize(exec, castedThis->globalObject(), 0) : jsNull();
    return result;
}