JSObject* JSTestObjPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSTestObj>(exec, globalObject);
}
