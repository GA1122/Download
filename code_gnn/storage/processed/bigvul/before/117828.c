static void longLongSequenceAttrAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.longLongSequenceAttr._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    Vector<long long> v = toNativeArray<long long>(value);
    imp->setLongLongSequenceAttr(v);
    return;
}
