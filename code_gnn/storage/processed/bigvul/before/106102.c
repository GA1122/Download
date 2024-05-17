JSValue jsTestObjCachedAttribute2(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    if (JSValue cachedValue = castedThis->m_cachedAttribute2.get())
        return cachedValue;
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    JSValue result = impl->cachedAttribute2() ? impl->cachedAttribute2()->deserialize(exec, castedThis->globalObject(), 0) : jsNull();
    castedThis->m_cachedAttribute2.set(exec->globalData(), castedThis, result);
    return result;
}
