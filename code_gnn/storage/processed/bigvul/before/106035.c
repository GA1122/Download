JSValue JSTestMediaQueryListListener::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestMediaQueryListListenerConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}
