static void shortSequenceAttrAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.shortSequenceAttr._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    Vector<short> v = toNativeArray<short>(value);
    imp->setShortSequenceAttr(v);
    return;
}
