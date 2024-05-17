void setJSTestObjWithScriptStateAttributeRaises(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(thisObject);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    ExceptionCode ec = 0;
    impl->setWithScriptStateAttributeRaises(exec, toTestObj(value), ec);
    setDOMException(exec, ec);
}
