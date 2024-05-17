bool JSTestObj::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    JSTestObj* thisObject = jsCast<JSTestObj*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSTestObj, Base>(exec, &JSTestObjTable, thisObject, propertyName, descriptor);
}
