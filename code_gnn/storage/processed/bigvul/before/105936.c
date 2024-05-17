bool JSTestActiveDOMObjectPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    JSTestActiveDOMObjectPrototype* thisObject = jsCast<JSTestActiveDOMObjectPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSTestActiveDOMObjectPrototypeTable, thisObject, propertyName, descriptor);
}
