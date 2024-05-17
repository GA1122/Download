bool JSTestSerializedScriptValueInterfacePrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    JSTestSerializedScriptValueInterfacePrototype* thisObject = jsCast<JSTestSerializedScriptValueInterfacePrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSTestSerializedScriptValueInterfacePrototypeTable, thisObject, propertyName, slot);
}
