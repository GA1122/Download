JSValue jsTestSerializedScriptValueInterfaceCachedValue(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestSerializedScriptValueInterface* castedThis = jsCast<JSTestSerializedScriptValueInterface*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    if (JSValue cachedValue = castedThis->m_cachedValue.get())
        return cachedValue;
    TestSerializedScriptValueInterface* impl = static_cast<TestSerializedScriptValueInterface*>(castedThis->impl());
    JSValue result = impl->cachedValue() ? impl->cachedValue()->deserialize(exec, castedThis->globalObject(), 0) : jsNull();
    castedThis->m_cachedValue.set(exec->globalData(), castedThis, result);
    return result;
}
