EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionGetSVGDocument(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSTestObj::s_info))
        return throwVMTypeError(exec);
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSTestObj::s_info);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    ExceptionCode ec = 0;
    if (!shouldAllowAccessToNode(exec, impl->getSVGDocument(ec)))
        return JSValue::encode(jsNull());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->getSVGDocument(ec)));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}
