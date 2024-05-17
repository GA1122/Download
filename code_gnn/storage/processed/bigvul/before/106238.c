void setJSTestObjUnsignedShortAttr(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(thisObject);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    impl->setUnsignedShortAttr(value.toUInt32(exec));
}
