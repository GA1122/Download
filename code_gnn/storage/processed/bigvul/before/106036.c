bool JSTestMediaQueryListListenerConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSTestMediaQueryListListenerConstructor, JSDOMWrapper>(exec, &JSTestMediaQueryListListenerConstructorTable, jsCast<JSTestMediaQueryListListenerConstructor*>(object), propertyName, descriptor);
}
