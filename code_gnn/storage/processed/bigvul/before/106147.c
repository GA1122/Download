EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionWithScriptArgumentsAndCallStack(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSTestObj::s_info))
        return throwVMTypeError(exec);
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSTestObj::s_info);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    RefPtr<ScriptArguments> scriptArguments(createScriptArguments(exec, 0));
    RefPtr<ScriptCallStack> callStack(createScriptCallStackForInspector(exec));
    impl->withScriptArgumentsAndCallStack(scriptArguments, callStack);
    return JSValue::encode(jsUndefined());
}
