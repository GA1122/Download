static v8::Handle<v8::Value> strawberryAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.strawberry._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8::Integer::New(imp->blueberry());
}