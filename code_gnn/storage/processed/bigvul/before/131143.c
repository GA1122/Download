static void withScriptStateObjExceptionMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "withScriptStateObjException", "TestObject", info.Holder(), info.GetIsolate());
    TestObject* imp = V8TestObject::toNative(info.Holder());
    ScriptState* currentState = ScriptState::current();
    if (!currentState)
        return;
    ScriptState& state = *currentState;
    RefPtr<TestObject> result = imp->withScriptStateObjException(&state, exceptionState);
    if (exceptionState.throwIfNeeded())
        return;
    if (state.hadException()) {
        v8::Local<v8::Value> exception = state.exception();
        state.clearException();
        throwError(exception, info.GetIsolate());
        return;
    }
    v8SetReturnValue(info, result.release());
}
