static v8::Handle<v8::Value> unsignedLongLongSequenceAttrAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.unsignedLongLongSequenceAttr._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8Array(imp->unsignedLongLongSequenceAttr(), info.GetIsolate());
}
