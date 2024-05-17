static void callWithScriptStateVoidMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    ScriptState* currentState = ScriptState::current();
    if (!currentState)
        return;
    ScriptState& state = *currentState;
    imp->callWithScriptStateVoidMethod(&state);
    if (state.hadException()) {
        v8::Local<v8::Value> exception = state.exception();
        state.clearException();
        throwError(exception, info.GetIsolate());
        return;
    }
}
