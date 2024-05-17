EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionConditionalMethod1(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSTestObj::s_info))
        return throwVMTypeError(exec);
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSTestObj::s_info);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());

    JSC::JSValue result = jsString(exec, impl->conditionalMethod1());
    return JSValue::encode(result);
}
