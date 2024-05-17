JSObject* JSTestCustomNamedGetterPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSTestCustomNamedGetter>(exec, globalObject);
}
