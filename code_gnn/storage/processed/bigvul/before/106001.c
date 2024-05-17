JSValue JSTestInterface::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestInterfaceConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}
