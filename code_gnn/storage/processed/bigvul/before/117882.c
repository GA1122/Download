static void unsignedIntSequenceAttrAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.unsignedIntSequenceAttr._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    Vector<unsigned int> v = toNativeArray<unsigned int>(value);
    imp->setUnsignedIntSequenceAttr(v);
    return;
}
