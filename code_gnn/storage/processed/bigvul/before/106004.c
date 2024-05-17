bool JSTestInterface::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    JSTestInterface* thisObject = jsCast<JSTestInterface*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSTestInterface, Base>(exec, &JSTestInterfaceTable, thisObject, propertyName, descriptor);
}
