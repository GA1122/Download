static void booleanSequenceAttrAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.booleanSequenceAttr._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    Vector<boolean> v = toNativeArray<boolean>(value);
    imp->setBooleanSequenceAttr(v);
    return;
}
