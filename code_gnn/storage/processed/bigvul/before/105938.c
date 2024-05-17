bool JSTestActiveDOMObjectConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSTestActiveDOMObjectConstructor, JSDOMWrapper>(exec, &JSTestActiveDOMObjectConstructorTable, jsCast<JSTestActiveDOMObjectConstructor*>(cell), propertyName, slot);
}
