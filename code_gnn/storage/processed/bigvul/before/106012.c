EncodedJSValue JSC_HOST_CALL jsTestInterfacePrototypeFunctionSupplementalMethod1(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSTestInterface::s_info))
        return throwVMTypeError(exec);
    JSTestInterface* castedThis = jsCast<JSTestInterface*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSTestInterface::s_info);
    TestInterface* impl = static_cast<TestInterface*>(castedThis->impl());
    TestSupplemental::supplementalMethod1(impl);
    return JSValue::encode(jsUndefined());
}
