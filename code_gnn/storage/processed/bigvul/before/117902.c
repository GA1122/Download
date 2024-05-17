static v8::Handle<v8::Value> withScriptExecutionContextAndScriptStateObjExceptionCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.withScriptExecutionContextAndScriptStateObjException");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    ExceptionCode ec = 0;
    {
    EmptyScriptState state;
    ScriptExecutionContext* scriptContext = getScriptExecutionContext();
    if (!scriptContext)
        return v8::Undefined();
    RefPtr<TestObj> result = imp->withScriptExecutionContextAndScriptStateObjException(&state, scriptContext, ec);
    if (UNLIKELY(ec))
        goto fail;
    if (state.hadException())
        return throwError(state.exception(), args.GetIsolate());
    return toV8(result.release(), args.GetIsolate());
    }
    fail:
    V8Proxy::setDOMException(ec, args.GetIsolate());
    return v8::Handle<v8::Value>();
}
