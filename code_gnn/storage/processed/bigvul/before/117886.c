static void unsignedLongLongSequenceAttrAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.unsignedLongLongSequenceAttr._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    Vector<unsigned long long> v = toNativeArray<unsigned long long>(value);
    imp->setUnsignedLongLongSequenceAttr(v);
    return;
}
