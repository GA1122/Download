bool JSTestActiveDOMObjectConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSTestActiveDOMObjectConstructor, JSDOMWrapper>(exec, &JSTestActiveDOMObjectConstructorTable, jsCast<JSTestActiveDOMObjectConstructor*>(object), propertyName, descriptor);
}
