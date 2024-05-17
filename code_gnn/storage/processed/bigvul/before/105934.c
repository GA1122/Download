JSValue JSTestActiveDOMObject::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestActiveDOMObjectConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}
