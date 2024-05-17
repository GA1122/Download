bool JSTestInterfaceConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticPropertyDescriptor<JSTestInterfaceConstructor, JSDOMWrapper>(exec, &JSTestInterfaceConstructorTable, jsCast<JSTestInterfaceConstructor*>(object), propertyName, descriptor);
}
