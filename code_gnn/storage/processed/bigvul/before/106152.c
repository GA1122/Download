EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionWithScriptStateObj(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSTestObj::s_info))
        return throwVMTypeError(exec);
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSTestObj::s_info);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->withScriptStateObj(exec)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    return JSValue::encode(result);
}
