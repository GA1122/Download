static void unsignedShortAttrAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.unsignedShortAttr._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    int v = toUInt32(value);
    imp->setUnsignedShortAttr(v);
    return;
}
