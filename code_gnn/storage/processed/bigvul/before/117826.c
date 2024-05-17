static void longLongAttrAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.longLongAttr._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    long long v = toInt64(value);
    imp->setLongLongAttr(WTF::getPtr(v));
    return;
}
