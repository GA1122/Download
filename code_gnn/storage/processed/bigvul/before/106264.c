bool JSTestSerializedScriptValueInterfaceConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSTestSerializedScriptValueInterfaceConstructor, JSDOMWrapper>(exec, &JSTestSerializedScriptValueInterfaceConstructorTable, jsCast<JSTestSerializedScriptValueInterfaceConstructor*>(cell), propertyName, slot);
}
