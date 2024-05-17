JSValue jsTestSerializedScriptValueInterfaceConstructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestSerializedScriptValueInterface* domObject = jsCast<JSTestSerializedScriptValueInterface*>(asObject(slotBase));
    return JSTestSerializedScriptValueInterface::getConstructor(exec, domObject->globalObject());
}
