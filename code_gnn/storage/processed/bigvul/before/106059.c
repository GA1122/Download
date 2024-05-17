bool JSTestNamedConstructorConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSTestNamedConstructorConstructor, JSDOMWrapper>(exec, &JSTestNamedConstructorConstructorTable, jsCast<JSTestNamedConstructorConstructor*>(object), propertyName, descriptor);
}
