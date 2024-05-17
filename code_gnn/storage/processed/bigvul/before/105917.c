bool JSFloat64ArrayConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSFloat64ArrayConstructor, JSDOMWrapper>(exec, &JSFloat64ArrayConstructorTable, jsCast<JSFloat64ArrayConstructor*>(object), propertyName, descriptor);
}
