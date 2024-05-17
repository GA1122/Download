static v8::Handle<v8::Value> withScriptExecutionContextAndScriptStateAttributeRaisesAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.withScriptExecutionContextAndScriptStateAttributeRaises._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    ExceptionCode ec = 0;
    ScriptState* state = ScriptState::current();
    if (!state)
        return v8::Undefined();
    ScriptExecutionContext* scriptContext = getScriptExecutionContext();
    if (!scriptContext)
        return v8::Undefined();
    RefPtr<TestObj> v = imp->withScriptExecutionContextAndScriptStateAttributeRaises(state, scriptContext, ec);
    if (UNLIKELY(ec)) {
        V8Proxy::setDOMException(ec, info.GetIsolate());
        return v8::Handle<v8::Value>();
    }
    if (state.hadException())
        return throwError(state.exception(), info.GetIsolate());
    return toV8(v.release(), info.GetIsolate());
}
