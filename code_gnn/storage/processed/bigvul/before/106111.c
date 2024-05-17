EncodedJSValue JSC_HOST_CALL jsTestObjConstructorFunctionClassMethodWithOptional(ExecState* exec)
{

    size_t argsCount = exec->argumentCount();
    if (argsCount <= 0) {

        JSC::JSValue result = jsNumber(TestObj::classMethodWithOptional());
        return JSValue::encode(result);
    }

    int arg(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = jsNumber(TestObj::classMethodWithOptional(arg));
    return JSValue::encode(result);
}
