bool JSTestCustomNamedGetterConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSTestCustomNamedGetterConstructor, JSDOMWrapper>(exec, &JSTestCustomNamedGetterConstructorTable, jsCast<JSTestCustomNamedGetterConstructor*>(object), propertyName, descriptor);
}
