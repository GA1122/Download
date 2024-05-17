static void voidSequenceAttrAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.voidSequenceAttr._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    Vector<void> v = toNativeArray<void>(value);
    imp->setVoidSequenceAttr(v);
    return;
}
