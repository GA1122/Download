void JSTestInterface::put(JSCell* cell, ExecState* exec, const Identifier& propertyName, JSValue value, PutPropertySlot& slot)
{
    JSTestInterface* thisObject = jsCast<JSTestInterface*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    if (thisObject->putDelegate(exec, propertyName, value, slot))
        return;
    lookupPut<JSTestInterface, Base>(exec, propertyName, value, &JSTestInterfaceTable, thisObject, slot);
}
