static v8::Handle<v8::Value> sequenceAttrAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.sequenceAttr._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8Array(imp->sequenceAttr(), info.GetIsolate());
}
