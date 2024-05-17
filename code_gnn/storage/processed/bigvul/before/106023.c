void setJSTestInterfaceSupplementalStr2(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestInterface* castedThis = jsCast<JSTestInterface*>(thisObject);
    TestInterface* impl = static_cast<TestInterface*>(castedThis->impl());
    TestSupplemental::setSupplementalStr2(impl, ustringToString(value.isEmpty() ? UString() : value.toString(exec)->value(exec)));
}
