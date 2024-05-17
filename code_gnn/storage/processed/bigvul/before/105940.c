bool JSTestActiveDOMObject::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    JSTestActiveDOMObject* thisObject = jsCast<JSTestActiveDOMObject*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSTestActiveDOMObject, Base>(exec, &JSTestActiveDOMObjectTable, thisObject, propertyName, slot);
}
