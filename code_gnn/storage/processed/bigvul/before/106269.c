JSValue jsTestSerializedScriptValueInterfaceCachedReadonlyValue(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestSerializedScriptValueInterface* castedThis = jsCast<JSTestSerializedScriptValueInterface*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    if (JSValue cachedValue = castedThis->m_cachedReadonlyValue.get())
        return cachedValue;
    TestSerializedScriptValueInterface* impl = static_cast<TestSerializedScriptValueInterface*>(castedThis->impl());
    JSValue result = impl->cachedReadonlyValue() ? impl->cachedReadonlyValue()->deserialize(exec, castedThis->globalObject(), 0) : jsNull();
    castedThis->m_cachedReadonlyValue.set(exec->globalData(), castedThis, result);
    return result;
}
