static void unsignedLongSequenceAttrAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.unsignedLongSequenceAttr._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    Vector<unsigned long> v = toNativeArray<unsigned long>(value);
    imp->setUnsignedLongSequenceAttr(v);
    return;
}
