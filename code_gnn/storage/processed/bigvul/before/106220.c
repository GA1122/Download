void setJSTestObjReflectedIntegralAttr(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(thisObject);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    impl->setIntegralAttribute(WebCore::HTMLNames::reflectedintegralattrAttr, value.toInt32(exec));
}