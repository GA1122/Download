JSValue jsTestInterfaceSupplementalStr2(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestInterface* castedThis = jsCast<JSTestInterface*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    TestInterface* impl = static_cast<TestInterface*>(castedThis->impl());
    JSValue result = jsString(exec, TestSupplemental::supplementalStr2(impl));
    return result;
}
