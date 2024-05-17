void setJSTestObjStringAttr(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(thisObject);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    impl->setStringAttr(ustringToString(value.isEmpty() ? UString() : value.toString(exec)->value(exec)));
}
