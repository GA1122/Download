bool JSTestEventTargetConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSTestEventTargetConstructor, JSDOMWrapper>(exec, &JSTestEventTargetConstructorTable, jsCast<JSTestEventTargetConstructor*>(cell), propertyName, slot);
}
