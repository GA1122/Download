static v8::Handle<v8::Value> reflectedCustomIntegralAttrAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.reflectedCustomIntegralAttr._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8::Integer::New(imp->getIntegralAttribute(WebCore::HTMLNames::customContentIntegralAttrAttr));
}
