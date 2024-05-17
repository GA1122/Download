static v8::Handle<v8::Value> withScriptExecutionContextAttributeAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.withScriptExecutionContextAttribute._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    ScriptExecutionContext* scriptContext = getScriptExecutionContext();
    if (!scriptContext)
        return v8::Undefined();
    return toV8(imp->withScriptExecutionContextAttribute(scriptContext), info.GetIsolate());
}
