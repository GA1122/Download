void JSTestObj::put(JSCell* cell, ExecState* exec, const Identifier& propertyName, JSValue value, PutPropertySlot& slot)
{
    JSTestObj* thisObject = jsCast<JSTestObj*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSTestObj, Base>(exec, propertyName, value, &JSTestObjTable, thisObject, slot);
}
