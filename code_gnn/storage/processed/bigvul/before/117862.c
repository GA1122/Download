static v8::Handle<v8::Value> reflectedUnsignedIntegralAttrAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.reflectedUnsignedIntegralAttr._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8::Integer::NewFromUnsigned(std::max(0, imp->getIntegralAttribute(WebCore::HTMLNames::reflectedunsignedintegralattrAttr)));
}
