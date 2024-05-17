bool JSTestSerializedScriptValueInterface::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    JSTestSerializedScriptValueInterface* thisObject = jsCast<JSTestSerializedScriptValueInterface*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSTestSerializedScriptValueInterface, Base>(exec, &JSTestSerializedScriptValueInterfaceTable, thisObject, propertyName, slot);
}
