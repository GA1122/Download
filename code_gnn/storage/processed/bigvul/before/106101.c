JSValue jsTestObjCachedAttribute1(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    if (JSValue cachedValue = castedThis->m_cachedAttribute1.get())
        return cachedValue;
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    JSValue result = impl->cachedAttribute1() ? impl->cachedAttribute1()->deserialize(exec, castedThis->globalObject(), 0) : jsNull();
    castedThis->m_cachedAttribute1.set(exec->globalData(), castedThis, result);
    return result;
}
