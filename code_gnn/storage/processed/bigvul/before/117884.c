static void unsignedLongLongAttrAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.unsignedLongLongAttr._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    unsigned long long v = toInt64(value);
    imp->setUnsignedLongLongAttr(WTF::getPtr(v));
    return;
}
