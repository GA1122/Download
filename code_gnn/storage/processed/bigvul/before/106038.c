bool JSTestMediaQueryListListener::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    JSTestMediaQueryListListener* thisObject = jsCast<JSTestMediaQueryListListener*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSTestMediaQueryListListener, Base>(exec, &JSTestMediaQueryListListenerTable, thisObject, propertyName, descriptor);
}
