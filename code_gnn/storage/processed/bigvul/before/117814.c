static v8::Handle<v8::Value> hashAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.hash._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8String(imp->hash(), info.GetIsolate());
}
