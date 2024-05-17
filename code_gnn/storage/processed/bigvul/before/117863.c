static void reflectedUnsignedIntegralAttrAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.reflectedUnsignedIntegralAttr._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    unsigned v = toUInt32(value);
    imp->setUnsignedIntegralAttribute(WebCore::HTMLNames::reflectedunsignedintegralattrAttr, v);
    return;
}
