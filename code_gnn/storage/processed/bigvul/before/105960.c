bool JSTestCustomNamedGetter::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    JSTestCustomNamedGetter* thisObject = jsCast<JSTestCustomNamedGetter*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    if (canGetItemsForName(exec, static_cast<TestCustomNamedGetter*>(thisObject->impl()), propertyName)) {
        slot.setCustom(thisObject, thisObject->nameGetter);
        return true;
    }
    return getStaticValueSlot<JSTestCustomNamedGetter, Base>(exec, &JSTestCustomNamedGetterTable, thisObject, propertyName, slot);
}
