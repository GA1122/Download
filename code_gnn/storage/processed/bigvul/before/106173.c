JSValue jsTestObjStringAttrWithGetterException(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(slotBase));
    ExceptionCode ec = 0;
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    JSC::JSValue result = jsString(exec, impl->stringAttrWithGetterException(ec));
    setDOMException(exec, ec);
    return result;
}
