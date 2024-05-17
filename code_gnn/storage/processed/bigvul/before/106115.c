JSValue jsTestObjCustomAttr(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(slotBase));
    return castedThis->customAttr(exec);
}
