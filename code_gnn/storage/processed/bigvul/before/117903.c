static v8::Handle<v8::Value> withScriptExecutionContextAndScriptStateWithSpacesAttributeAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.withScriptExecutionContextAndScriptStateWithSpacesAttribute._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    ScriptState* state = ScriptState::current();
    if (!state)
        return v8::Undefined();
    ScriptExecutionContext* scriptContext = getScriptExecutionContext();
    if (!scriptContext)
        return v8::Undefined();
    return toV8(imp->withScriptExecutionContextAndScriptStateWithSpacesAttribute(state, scriptContext), info.GetIsolate());
}
