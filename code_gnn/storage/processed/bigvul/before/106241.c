void setJSTestObjWithScriptArgumentsAndCallStackAttribute(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(thisObject);
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    RefPtr<ScriptCallStack> callStack(createScriptCallStackForInspector(exec));
    impl->setWithScriptArgumentsAndCallStackAttribute(callStack, toTestObj(value));
}
