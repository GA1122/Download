static v8::Handle<v8::Value> readOnlyIntAttrAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.readOnlyIntAttr._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8::Integer::New(imp->readOnlyIntAttr());
}
