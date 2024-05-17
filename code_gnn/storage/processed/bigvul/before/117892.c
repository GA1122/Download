static void unsignedShortSequenceAttrAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.unsignedShortSequenceAttr._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    Vector<unsigned short> v = toNativeArray<unsigned short>(value);
    imp->setUnsignedShortSequenceAttr(v);
    return;
}
