JSObject* JSTestNamedConstructorPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSTestNamedConstructor>(exec, globalObject);
}
