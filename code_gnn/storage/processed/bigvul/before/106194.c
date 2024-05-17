void setJSTestObjAttrWithGetterException(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(thisObject);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    ExceptionCode ec = 0;
    impl->setAttrWithGetterException(value.toInt32(exec), ec);
    setDOMException(exec, ec);
}
