static void callWithScriptStateScriptArgumentsVoidMethodOptionalBooleanArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    if (UNLIKELY(info.Length() <= 0)) {
        ScriptState* currentState = ScriptState::current();
        if (!currentState)
            return;
        ScriptState& state = *currentState;
        RefPtr<ScriptArguments> scriptArguments(createScriptArguments(info, 1));
        imp->callWithScriptStateScriptArgumentsVoidMethodOptionalBooleanArg(&state, scriptArguments.release());
        if (state.hadException()) {
            v8::Local<v8::Value> exception = state.exception();
            state.clearException();
            throwError(exception, info.GetIsolate());
            return;
        }
        return;
    }
    V8TRYCATCH_VOID(bool, optionalBooleanArg, info[0]->BooleanValue());
    ScriptState* currentState = ScriptState::current();
    if (!currentState)
        return;
    ScriptState& state = *currentState;
    RefPtr<ScriptArguments> scriptArguments(createScriptArguments(info, 1));
    imp->callWithScriptStateScriptArgumentsVoidMethodOptionalBooleanArg(&state, scriptArguments.release(), optionalBooleanArg);
    if (state.hadException()) {
        v8::Local<v8::Value> exception = state.exception();
        state.clearException();
        throwError(exception, info.GetIsolate());
        return;
    }
}
