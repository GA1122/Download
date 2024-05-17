JSValue jsTestInterfaceSupplementalStr3(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestInterface* castedThis = jsCast<JSTestInterface*>(asObject(slotBase));
    return castedThis->supplementalStr3(exec);
}
