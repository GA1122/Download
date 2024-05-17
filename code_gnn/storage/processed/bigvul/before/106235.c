void setJSTestObjUnsignedLongLongAttr(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(thisObject);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    impl->setUnsignedLongLongAttr(static_cast<unsigned long long>(value.toInteger(exec)));
}
