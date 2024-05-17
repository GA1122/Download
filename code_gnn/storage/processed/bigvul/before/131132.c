static void withExecutionContextAndScriptStateWithSpacesMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    ScriptState* currentState = ScriptState::current();
    if (!currentState)
        return;
    ScriptState& state = *currentState;
    ExecutionContext* scriptContext = currentExecutionContext(info.GetIsolate());
    RefPtr<TestObject> result = imp->withExecutionContextAndScriptStateWithSpaces(&state, scriptContext);
    if (state.hadException()) {
        v8::Local<v8::Value> exception = state.exception();
        state.clearException();
        throwError(exception, info.GetIsolate());
        return;
    }
    v8SetReturnValue(info, result.release());
}
