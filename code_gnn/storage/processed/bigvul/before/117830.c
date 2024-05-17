static void longSequenceAttrAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.longSequenceAttr._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    Vector<long> v = toNativeArray<long>(value);
    imp->setLongSequenceAttr(v);
    return;
}
