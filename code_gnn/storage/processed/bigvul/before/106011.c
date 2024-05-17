EncodedJSValue JSC_HOST_CALL jsTestInterfaceConstructorFunctionSupplementalMethod4(ExecState* exec)
{
    TestSupplemental::supplementalMethod4();
    return JSValue::encode(jsUndefined());
}
