static void doubleSequenceAttrAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.doubleSequenceAttr._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    Vector<double> v = toNativeArray<double>(value);
    imp->setDoubleSequenceAttr(v);
    return;
}
