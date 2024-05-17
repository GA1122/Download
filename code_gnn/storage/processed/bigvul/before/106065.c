JSValue jsTestNamedConstructorConstructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestNamedConstructor* domObject = jsCast<JSTestNamedConstructor*>(asObject(slotBase));
    return JSTestNamedConstructor::getConstructor(exec, domObject->globalObject());
}
