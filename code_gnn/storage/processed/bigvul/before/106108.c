JSValue jsTestObjConditionalAttr6Constructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(slotBase));
    return JSTestObjectC::getConstructor(exec, castedThis);
}
