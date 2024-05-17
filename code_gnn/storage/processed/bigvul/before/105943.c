JSObject* JSTestActiveDOMObjectPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSTestActiveDOMObject>(exec, globalObject);
}
