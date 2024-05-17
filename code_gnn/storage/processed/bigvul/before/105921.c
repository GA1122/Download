bool JSFloat64ArrayPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    JSFloat64ArrayPrototype* thisObject = jsCast<JSFloat64ArrayPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, getJSFloat64ArrayPrototypeTable(exec), thisObject, propertyName, slot);
}
