static v8::Handle<v8::Value> booleanSequenceAttrAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.booleanSequenceAttr._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8Array(imp->booleanSequenceAttr(), info.GetIsolate());
}
