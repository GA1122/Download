static void withScriptExecutionContextAttributeAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.withScriptExecutionContextAttribute._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    TestObj* v = V8TestObj::HasInstance(value) ? V8TestObj::toNative(v8::Handle<v8::Object>::Cast(value)) : 0;
    ScriptExecutionContext* scriptContext = getScriptExecutionContext();
    if (!scriptContext)
        return;
    imp->setWithScriptExecutionContextAttribute(scriptContext, WTF::getPtr(v));
    return;
}
