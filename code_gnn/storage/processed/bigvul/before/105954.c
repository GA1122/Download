JSValue JSTestCustomNamedGetter::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestCustomNamedGetterConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}
