bool JSTestInterfacePrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    JSTestInterfacePrototype* thisObject = jsCast<JSTestInterfacePrototype*>(cell);
    return getStaticPropertySlot<JSTestInterfacePrototype, JSObject>(exec, &JSTestInterfacePrototypeTable, thisObject, propertyName, slot);
}
