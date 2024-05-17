static void withScriptStateAttributeAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.withScriptStateAttribute._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    int v = toInt32(value);
    ScriptState* state = ScriptState::current();
    if (!state)
        return;
    imp->setWithScriptStateAttribute(state, v);
    if (state.hadException())
        throwError(state.exception(), info.GetIsolate());
    return;
}
