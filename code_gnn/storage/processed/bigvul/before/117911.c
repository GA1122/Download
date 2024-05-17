static v8::Handle<v8::Value> withScriptStateAttributeAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.withScriptStateAttribute._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    ScriptState* state = ScriptState::current();
    if (!state)
        return v8::Undefined();
    return v8::Integer::New(imp->withScriptStateAttribute(state));
}
