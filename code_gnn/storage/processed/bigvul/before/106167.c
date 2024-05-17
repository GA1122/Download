JSValue jsTestObjReflectedUnsignedIntegralAttr(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    JSValue result = jsNumber(std::max(0, impl->getIntegralAttribute(WebCore::HTMLNames::reflectedunsignedintegralattrAttr)));
    return result;
}
