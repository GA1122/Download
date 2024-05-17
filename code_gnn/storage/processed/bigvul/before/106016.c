JSValue jsTestInterfaceSupplementalNode(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestInterface* castedThis = jsCast<JSTestInterface*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    TestInterface* impl = static_cast<TestInterface*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(TestSupplemental::supplementalNode(impl)));
    return result;
}
