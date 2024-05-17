EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithOptionalArg(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSTestObj::s_info))
        return throwVMTypeError(exec);
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSTestObj::s_info);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());

    size_t argsCount = exec->argumentCount();
    if (argsCount <= 0) {
        impl->methodWithOptionalArg();
        return JSValue::encode(jsUndefined());
    }

    int opt(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->methodWithOptionalArg(opt);
    return JSValue::encode(jsUndefined());
}
