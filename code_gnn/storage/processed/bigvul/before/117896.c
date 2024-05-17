static void withScriptArgumentsAndCallStackAttributeAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.withScriptArgumentsAndCallStackAttribute._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    TestObj* v = V8TestObj::HasInstance(value) ? V8TestObj::toNative(v8::Handle<v8::Object>::Cast(value)) : 0;
    RefPtr<ScriptCallStack> callStack(createScriptCallStackForInspector());
    if (!callStack)
        return v8::Undefined();
    imp->setWithScriptArgumentsAndCallStackAttribute(callStack, WTF::getPtr(v));
    return;
}
