bool JSTestSerializedScriptValueInterfacePrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    JSTestSerializedScriptValueInterfacePrototype* thisObject = jsCast<JSTestSerializedScriptValueInterfacePrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSTestSerializedScriptValueInterfacePrototypeTable, thisObject, propertyName, descriptor);
}
