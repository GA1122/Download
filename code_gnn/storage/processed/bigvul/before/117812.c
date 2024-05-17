static void floatSequenceAttrAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.floatSequenceAttr._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    Vector<float> v = toNativeArray<float>(value);
    imp->setFloatSequenceAttr(v);
    return;
}
