bool JSTestObjPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    JSTestObjPrototype* thisObject = jsCast<JSTestObjPrototype*>(cell);
    return getStaticPropertySlot<JSTestObjPrototype, JSObject>(exec, &JSTestObjPrototypeTable, thisObject, propertyName, slot);
}
