void setJSTestObjBooleanSequenceAttr(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(thisObject);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    impl->setBooleanSequenceAttr(toNativeArray<boolean>(exec, value));
}
