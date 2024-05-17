EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithCallbackAndOptionalArg(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSTestObj::s_info))
        return throwVMTypeError(exec);
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSTestObj::s_info);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    RefPtr<TestCallback> callback;
    if (exec->argumentCount() > 0 && !exec->argument(0).isUndefinedOrNull()) {
        if (!exec->argument(0).isFunction()) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return JSValue::encode(jsUndefined());
        }
        callback = JSTestCallback::create(asObject(exec->argument(0)), castedThis->globalObject());
    }
    impl->methodWithCallbackAndOptionalArg(callback);
    return JSValue::encode(jsUndefined());
}
