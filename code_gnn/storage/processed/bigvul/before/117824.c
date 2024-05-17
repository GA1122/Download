static void intSequenceAttrAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.intSequenceAttr._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    Vector<int> v = toNativeArray<int>(value);
    imp->setIntSequenceAttr(v);
    return;
}
