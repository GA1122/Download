JSObject* JSTestInterfacePrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSTestInterface>(exec, globalObject);
}
