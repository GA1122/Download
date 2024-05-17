void setJSTestObjUnsignedLongSequenceAttr(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(thisObject);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    impl->setUnsignedLongSequenceAttr(toNativeArray<unsigned long>(exec, value));
}
