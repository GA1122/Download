static v8::Handle<v8::Value> withScriptStateVoidCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.withScriptStateVoid");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    EmptyScriptState state;
    imp->withScriptStateVoid(&state);
    if (state.hadException())
        return throwError(state.exception(), args.GetIsolate());
    return v8::Handle<v8::Value>();
}
