static v8::Handle<v8::Value> doubleArrayAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.doubleArray._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8NumberArray(imp->doubleArray());
}
