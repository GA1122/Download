static v8::Handle<v8::Value> withScriptArgumentsAndCallStackAttributeAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.withScriptArgumentsAndCallStackAttribute._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    RefPtr<ScriptCallStack> callStack(createScriptCallStackForInspector());
    if (!callStack)
        return v8::Undefined();
    return toV8(imp->withScriptArgumentsAndCallStackAttribute(callStack), info.GetIsolate());
}
