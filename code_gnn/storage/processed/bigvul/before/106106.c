JSValue jsTestObjConditionalAttr4Constructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(slotBase));
    return JSTestObjectA::getConstructor(exec, castedThis);
}
