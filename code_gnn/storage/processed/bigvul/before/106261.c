bool JSTestSerializedScriptValueInterfaceConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSTestSerializedScriptValueInterfaceConstructor, JSDOMWrapper>(exec, &JSTestSerializedScriptValueInterfaceConstructorTable, jsCast<JSTestSerializedScriptValueInterfaceConstructor*>(object), propertyName, descriptor);
}
