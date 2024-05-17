JSValue JSTestNamedConstructor::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestNamedConstructorConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}
