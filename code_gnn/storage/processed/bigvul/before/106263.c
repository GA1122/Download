bool JSTestSerializedScriptValueInterface::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    JSTestSerializedScriptValueInterface* thisObject = jsCast<JSTestSerializedScriptValueInterface*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSTestSerializedScriptValueInterface, Base>(exec, &JSTestSerializedScriptValueInterfaceTable, thisObject, propertyName, descriptor);
}
