static v8::Handle<v8::Value> unsignedShortSequenceAttrAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.unsignedShortSequenceAttr._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8Array(imp->unsignedShortSequenceAttr(), info.GetIsolate());
}
