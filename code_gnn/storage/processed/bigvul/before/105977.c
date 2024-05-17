bool JSTestEventTargetPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    JSTestEventTargetPrototype* thisObject = jsCast<JSTestEventTargetPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSTestEventTargetPrototypeTable, thisObject, propertyName, descriptor);
}
