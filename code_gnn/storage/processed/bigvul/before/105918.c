bool JSFloat64Array::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    JSFloat64Array* thisObject = jsCast<JSFloat64Array*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    bool ok;
    unsigned index = propertyName.toUInt32(ok);
    if (ok && index < static_cast<Float64Array*>(thisObject->impl())->length()) {
        descriptor.setDescriptor(thisObject->getByIndex(exec, index), DontDelete);
        return true;
    }
    return getStaticValueDescriptor<JSFloat64Array, Base>(exec, getJSFloat64ArrayTable(exec), thisObject, propertyName, descriptor);
}
