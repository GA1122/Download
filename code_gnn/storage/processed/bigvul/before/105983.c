bool JSTestEventTarget::getOwnPropertySlotByIndex(JSCell* cell, ExecState* exec, unsigned propertyName, PropertySlot& slot)
{
    JSTestEventTarget* thisObject = jsCast<JSTestEventTarget*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    if (propertyName < static_cast<TestEventTarget*>(thisObject->impl())->length()) {
        slot.setCustomIndex(thisObject, propertyName, thisObject->indexGetter);
        return true;
    }
    return thisObject->methodTable()->getOwnPropertySlot(thisObject, exec, Identifier::from(exec, propertyName), slot);
}
