void JSFloat64Array::putByIndex(JSCell* cell, ExecState* exec, unsigned propertyName, JSValue value, bool)
{
    JSFloat64Array* thisObject = jsCast<JSFloat64Array*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    thisObject->indexSetter(exec, propertyName, value);
    return;
}
