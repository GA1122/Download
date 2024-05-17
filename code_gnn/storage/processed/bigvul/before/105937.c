bool JSTestActiveDOMObject::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    JSTestActiveDOMObject* thisObject = jsCast<JSTestActiveDOMObject*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    if (!shouldAllowAccessToFrame(exec, thisObject->impl()->frame()))
        return false;
    return getStaticValueDescriptor<JSTestActiveDOMObject, Base>(exec, &JSTestActiveDOMObjectTable, thisObject, propertyName, descriptor);
}
