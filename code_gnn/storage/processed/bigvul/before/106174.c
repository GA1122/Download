JSValue jsTestObjStringAttrWithSetterException(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    JSValue result = jsString(exec, impl->stringAttrWithSetterException());
    return result;
}
