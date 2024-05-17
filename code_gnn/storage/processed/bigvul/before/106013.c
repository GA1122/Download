EncodedJSValue JSC_HOST_CALL jsTestInterfacePrototypeFunctionSupplementalMethod3(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSTestInterface::s_info))
        return throwVMTypeError(exec);
    JSTestInterface* castedThis = jsCast<JSTestInterface*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSTestInterface::s_info);
    return JSValue::encode(castedThis->supplementalMethod3(exec));
}
