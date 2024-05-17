static v8::Handle<v8::Value> withScriptExecutionContextAttributeRaisesAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.withScriptExecutionContextAttributeRaises._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    ExceptionCode ec = 0;
    ScriptExecutionContext* scriptContext = getScriptExecutionContext();
    if (!scriptContext)
        return v8::Undefined();
    RefPtr<TestObj> v = imp->withScriptExecutionContextAttributeRaises(scriptContext, ec);
    if (UNLIKELY(ec)) {
        V8Proxy::setDOMException(ec, info.GetIsolate());
        return v8::Handle<v8::Value>();
    }
    return toV8(v.release(), info.GetIsolate());
}
