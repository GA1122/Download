EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithOptionalStringIsNullString(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSTestObj::s_info))
        return throwVMTypeError(exec);
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSTestObj::s_info);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    const String& str(ustringToString(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsNullString).isEmpty() ? UString() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsNullString).toString(exec)->value(exec)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->methodWithOptionalStringIsNullString(str);
    return JSValue::encode(jsUndefined());
}