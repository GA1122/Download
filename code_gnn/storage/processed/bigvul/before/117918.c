static v8::Handle<v8::Value> withScriptStateVoidExceptionCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.withScriptStateVoidException");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    ExceptionCode ec = 0;
    {
    EmptyScriptState state;
    imp->withScriptStateVoidException(&state, ec);
    if (UNLIKELY(ec))
        goto fail;
    if (state.hadException())
        return throwError(state.exception(), args.GetIsolate());
    return v8::Handle<v8::Value>();
    }
    fail:
    V8Proxy::setDOMException(ec, args.GetIsolate());
    return v8::Handle<v8::Value>();
}
