JSValue jsTestObjConstructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestObj* domObject = jsCast<JSTestObj*>(asObject(slotBase));
    return JSTestObj::getConstructor(exec, domObject->globalObject());
}
