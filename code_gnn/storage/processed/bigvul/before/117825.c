static v8::Handle<v8::Value> longLongAttrAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.longLongAttr._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8::Number::New(static_cast<double>(imp->longLongAttr()));
}
