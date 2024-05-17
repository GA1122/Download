EncodedJSValue JSC_HOST_CALL jsTestObjConstructorFunctionOverloadedMethod1(ExecState* exec)
{
    size_t argsCount = exec->argumentCount();
    if (argsCount == 1)
        return jsTestObjConstructorFunctionOverloadedMethod11(exec);
    JSValue arg0(exec->argument(0));
    if ((argsCount == 1 && (arg0.isUndefinedOrNull() || arg0.isString() || arg0.isObject())))
        return jsTestObjConstructorFunctionOverloadedMethod12(exec);
    return throwVMTypeError(exec);
}
