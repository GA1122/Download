bool JSFloat64ArrayConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSFloat64ArrayConstructor, JSDOMWrapper>(exec, &JSFloat64ArrayConstructorTable, jsCast<JSFloat64ArrayConstructor*>(cell), propertyName, slot);
}
