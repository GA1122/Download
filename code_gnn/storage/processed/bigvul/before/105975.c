JSValue JSTestEventTarget::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestEventTargetConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}
