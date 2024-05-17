JSValue jsTestObjWithScriptExecutionContextAttributeRaises(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(slotBase));
    ExceptionCode ec = 0;
    ScriptExecutionContext* scriptContext = jsCast<JSDOMGlobalObject*>(exec->lexicalGlobalObject())->scriptExecutionContext();
    if (!scriptContext)
        return jsUndefined();
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->withScriptExecutionContextAttributeRaises(scriptContext, ec)));
    setDOMException(exec, ec);
    return result;
}
