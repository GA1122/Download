static v8::Handle<v8::Value> withScriptExecutionContextAndScriptStateWithSpacesCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.withScriptExecutionContextAndScriptStateWithSpaces");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    EmptyScriptState state;
    ScriptExecutionContext* scriptContext = getScriptExecutionContext();
    if (!scriptContext)
        return v8::Undefined();
    RefPtr<TestObj> result = imp->withScriptExecutionContextAndScriptStateWithSpaces(&state, scriptContext);
    if (state.hadException())
        return throwError(state.exception(), args.GetIsolate());
    return toV8(result.release(), args.GetIsolate());
}
