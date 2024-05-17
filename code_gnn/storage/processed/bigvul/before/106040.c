bool JSTestMediaQueryListListenerPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    JSTestMediaQueryListListenerPrototype* thisObject = jsCast<JSTestMediaQueryListListenerPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSTestMediaQueryListListenerPrototypeTable, thisObject, propertyName, slot);
}
