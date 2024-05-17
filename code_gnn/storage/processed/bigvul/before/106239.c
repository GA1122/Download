void setJSTestObjUnsignedShortSequenceAttr(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(thisObject);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    impl->setUnsignedShortSequenceAttr(toNativeArray<unsigned short>(exec, value));
}
