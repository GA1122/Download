JSValue jsTestActiveDOMObjectExcitingAttr(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestActiveDOMObject* castedThis = jsCast<JSTestActiveDOMObject*>(asObject(slotBase));
    if (!castedThis->allowsAccessFrom(exec))
        return jsUndefined();
    UNUSED_PARAM(exec);
    TestActiveDOMObject* impl = static_cast<TestActiveDOMObject*>(castedThis->impl());
    JSValue result = jsNumber(impl->excitingAttr());
    return result;
}
