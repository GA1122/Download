bool JSTestEventTargetPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    JSTestEventTargetPrototype* thisObject = jsCast<JSTestEventTargetPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSTestEventTargetPrototypeTable, thisObject, propertyName, slot);
}
