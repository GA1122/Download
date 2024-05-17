static v8::Handle<v8::Value> withScriptExecutionContextAndScriptStateCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.withScriptExecutionContextAndScriptState");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    EmptyScriptState state;
    ScriptExecutionContext* scriptContext = getScriptExecutionContext();
    if (!scriptContext)
        return v8::Undefined();
    imp->withScriptExecutionContextAndScriptState(&state, scriptContext);
    if (state.hadException())
        return throwError(state.exception(), args.GetIsolate());
    return v8::Handle<v8::Value>();
}
