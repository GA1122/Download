static v8::Handle<v8::Value> withScriptStateObjCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.withScriptStateObj");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    EmptyScriptState state;
    RefPtr<TestObj> result = imp->withScriptStateObj(&state);
    if (state.hadException())
        return throwError(state.exception(), args.GetIsolate());
    return toV8(result.release(), args.GetIsolate());
}
