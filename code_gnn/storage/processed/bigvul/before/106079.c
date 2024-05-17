bool JSTestObjConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticPropertyDescriptor<JSTestObjConstructor, JSDOMWrapper>(exec, &JSTestObjConstructorTable, jsCast<JSTestObjConstructor*>(object), propertyName, descriptor);
}
