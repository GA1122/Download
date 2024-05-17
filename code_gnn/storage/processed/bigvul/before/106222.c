void setJSTestObjReflectedStringAttr(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(thisObject);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::customContentStringAttrAttr, valueToStringWithNullCheck(exec, value));
}
