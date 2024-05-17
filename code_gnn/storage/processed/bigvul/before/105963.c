JSValue jsTestCustomNamedGetterConstructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestCustomNamedGetter* domObject = jsCast<JSTestCustomNamedGetter*>(asObject(slotBase));
    return JSTestCustomNamedGetter::getConstructor(exec, domObject->globalObject());
}
