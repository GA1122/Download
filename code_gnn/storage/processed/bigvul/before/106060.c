bool JSTestNamedConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    JSTestNamedConstructor* thisObject = jsCast<JSTestNamedConstructor*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSTestNamedConstructor, Base>(exec, &JSTestNamedConstructorTable, thisObject, propertyName, descriptor);
}
