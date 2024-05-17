void setJSTestInterfaceSupplementalNode(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestInterface* castedThis = jsCast<JSTestInterface*>(thisObject);
    TestInterface* impl = static_cast<TestInterface*>(castedThis->impl());
    TestSupplemental::setSupplementalNode(impl, toNode(value));
}
