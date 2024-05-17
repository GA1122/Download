bool JSTestNamedConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    JSTestNamedConstructor* thisObject = jsCast<JSTestNamedConstructor*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSTestNamedConstructor, Base>(exec, &JSTestNamedConstructorTable, thisObject, propertyName, slot);
}
