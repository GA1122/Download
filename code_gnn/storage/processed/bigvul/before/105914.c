JSValue JSFloat64Array::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSFloat64ArrayConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}
