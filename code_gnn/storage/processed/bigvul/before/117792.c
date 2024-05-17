static v8::Handle<v8::Value> conditionalAttr3AttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.conditionalAttr3._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8::Integer::New(imp->conditionalAttr3());
}
