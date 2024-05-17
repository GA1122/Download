static v8::Handle<v8::Value> enabledAtRuntimeAttr1AttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.enabledAtRuntimeAttr1._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8::Integer::New(imp->enabledAtRuntimeAttr1());
}
