bool JSTestObj::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    JSTestObj* thisObject = jsCast<JSTestObj*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSTestObj, Base>(exec, &JSTestObjTable, thisObject, propertyName, slot);
}
