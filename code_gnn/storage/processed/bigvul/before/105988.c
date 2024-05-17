JSObject* JSTestEventTargetPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSTestEventTarget>(exec, globalObject);
}
