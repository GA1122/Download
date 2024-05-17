void setJSTestObjUnsignedLongLongSequenceAttr(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(thisObject);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    impl->setUnsignedLongLongSequenceAttr(toNativeArray<unsigned long long>(exec, value));
}
