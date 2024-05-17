JSValue jsFloat64ArrayConstructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSFloat64Array* domObject = jsCast<JSFloat64Array*>(asObject(slotBase));
    return JSFloat64Array::getConstructor(exec, domObject->globalObject());
}
