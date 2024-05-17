bool JSTestCustomNamedGetterPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    JSTestCustomNamedGetterPrototype* thisObject = jsCast<JSTestCustomNamedGetterPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSTestCustomNamedGetterPrototypeTable, thisObject, propertyName, slot);
}
