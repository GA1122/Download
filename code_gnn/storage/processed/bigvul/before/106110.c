EncodedJSValue JSC_HOST_CALL jsTestObjConstructorFunctionClassMethod(ExecState* exec)
{
    TestObj::classMethod();
    return JSValue::encode(jsUndefined());
}
