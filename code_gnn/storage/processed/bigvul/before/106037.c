bool JSTestMediaQueryListListenerPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    JSTestMediaQueryListListenerPrototype* thisObject = jsCast<JSTestMediaQueryListListenerPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSTestMediaQueryListListenerPrototypeTable, thisObject, propertyName, descriptor);
}
