static v8::Handle<v8::Value> floatArrayAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.floatArray._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8NumberArray(imp->floatArray());
}
