bool JSTestActiveDOMObjectPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    JSTestActiveDOMObjectPrototype* thisObject = jsCast<JSTestActiveDOMObjectPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSTestActiveDOMObjectPrototypeTable, thisObject, propertyName, slot);
}
