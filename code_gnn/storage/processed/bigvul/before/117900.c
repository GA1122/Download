static void withScriptExecutionContextAndScriptStateAttributeRaisesAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.withScriptExecutionContextAndScriptStateAttributeRaises._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    TestObj* v = V8TestObj::HasInstance(value) ? V8TestObj::toNative(v8::Handle<v8::Object>::Cast(value)) : 0;
    ExceptionCode ec = 0;
    ScriptState* state = ScriptState::current();
    if (!state)
        return;
    ScriptExecutionContext* scriptContext = getScriptExecutionContext();
    if (!scriptContext)
        return;
    imp->setWithScriptExecutionContextAndScriptStateAttributeRaises(state, scriptContext, WTF::getPtr(v), ec);
    if (UNLIKELY(ec))
        V8Proxy::setDOMException(ec, info.GetIsolate());
    if (state.hadException())
        throwError(state.exception(), info.GetIsolate());
    return;
}
