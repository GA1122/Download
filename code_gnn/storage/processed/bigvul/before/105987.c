JSValue jsTestEventTargetConstructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestEventTarget* domObject = jsCast<JSTestEventTarget*>(asObject(slotBase));
    return JSTestEventTarget::getConstructor(exec, domObject->globalObject());
}
