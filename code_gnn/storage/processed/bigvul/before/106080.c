bool JSTestObjPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    JSTestObjPrototype* thisObject = jsCast<JSTestObjPrototype*>(object);
    return getStaticPropertyDescriptor<JSTestObjPrototype, JSObject>(exec, &JSTestObjPrototypeTable, thisObject, propertyName, descriptor);
}
