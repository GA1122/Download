EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionCustomMethodWithArgs(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSTestObj::s_info))
        return throwVMTypeError(exec);
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSTestObj::s_info);
    return JSValue::encode(castedThis->customMethodWithArgs(exec));
}
