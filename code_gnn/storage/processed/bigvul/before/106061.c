bool JSTestNamedConstructorConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSTestNamedConstructorConstructor, JSDOMWrapper>(exec, &JSTestNamedConstructorConstructorTable, jsCast<JSTestNamedConstructorConstructor*>(cell), propertyName, slot);
}
