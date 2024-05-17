static v8::Handle<v8::Value> reflectedCustomBooleanAttrAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.reflectedCustomBooleanAttr._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8Boolean(imp->hasAttribute(WebCore::HTMLNames::customContentBooleanAttrAttr));
}