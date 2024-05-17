bool JSTestCustomNamedGetterConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSTestCustomNamedGetterConstructor, JSDOMWrapper>(exec, &JSTestCustomNamedGetterConstructorTable, jsCast<JSTestCustomNamedGetterConstructor*>(cell), propertyName, slot);
}
