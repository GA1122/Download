void setJSTestObjConditionalAttr3(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(thisObject);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    impl->setConditionalAttr3(value.toInt32(exec));
}
