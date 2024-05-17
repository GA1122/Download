static v8::Handle<v8::Value> excitingAttrAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestActiveDOMObject.excitingAttr._get");
    TestActiveDOMObject* imp = V8TestActiveDOMObject::toNative(info.Holder());
    return v8::Integer::New(imp->excitingAttr());
}
