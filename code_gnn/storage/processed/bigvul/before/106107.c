JSValue jsTestObjConditionalAttr5Constructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(slotBase));
    return JSTestObjectB::getConstructor(exec, castedThis);
}
