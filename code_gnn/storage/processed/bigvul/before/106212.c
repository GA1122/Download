void setJSTestObjLongLongAttr(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(thisObject);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    impl->setLongLongAttr(static_cast<long long>(value.toInteger(exec)));
}
