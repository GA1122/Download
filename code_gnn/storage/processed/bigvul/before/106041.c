bool JSTestMediaQueryListListener::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    JSTestMediaQueryListListener* thisObject = jsCast<JSTestMediaQueryListListener*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSTestMediaQueryListListener, Base>(exec, &JSTestMediaQueryListListenerTable, thisObject, propertyName, slot);
}
