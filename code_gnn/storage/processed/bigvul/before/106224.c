void setJSTestObjReflectedUnsignedIntegralAttr(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(thisObject);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    impl->setUnsignedIntegralAttribute(WebCore::HTMLNames::reflectedunsignedintegralattrAttr, value.toUInt32(exec));
}
