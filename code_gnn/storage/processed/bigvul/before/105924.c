void JSFloat64Array::put(JSCell* cell, ExecState* exec, const Identifier& propertyName, JSValue value, PutPropertySlot& slot)
{
    JSFloat64Array* thisObject = jsCast<JSFloat64Array*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    bool ok;
    unsigned index = propertyName.toUInt32(ok);
    if (ok) {
        thisObject->indexSetter(exec, index, value);
        return;
    }
    Base::put(thisObject, exec, propertyName, value, slot);
}
