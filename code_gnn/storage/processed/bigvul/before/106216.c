void setJSTestObjReflectedBooleanAttr(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(thisObject);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    impl->setBooleanAttribute(WebCore::HTMLNames::reflectedbooleanattrAttr, value.toBoolean(exec));
}
