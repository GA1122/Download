JSValue jsTestObjWithScriptStateAttributeRaises(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(slotBase));
    ExceptionCode ec = 0;
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->withScriptStateAttributeRaises(exec, ec)));
    setDOMException(exec, ec);
    return result;
}
