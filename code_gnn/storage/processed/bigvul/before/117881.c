static v8::Handle<v8::Value> unsignedIntSequenceAttrAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.unsignedIntSequenceAttr._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8Array(imp->unsignedIntSequenceAttr(), info.GetIsolate());
}
