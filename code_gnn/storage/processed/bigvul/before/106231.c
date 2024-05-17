void setJSTestObjStringAttrWithGetterException(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(thisObject);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    ExceptionCode ec = 0;
    impl->setStringAttrWithGetterException(ustringToString(value.isEmpty() ? UString() : value.toString(exec)->value(exec)), ec);
    setDOMException(exec, ec);
}
