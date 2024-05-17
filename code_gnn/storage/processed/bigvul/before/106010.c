JSValue jsTestInterfaceConstructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestInterface* domObject = jsCast<JSTestInterface*>(asObject(slotBase));
    return JSTestInterface::getConstructor(exec, domObject->globalObject());
}
