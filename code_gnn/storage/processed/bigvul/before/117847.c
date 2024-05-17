static v8::Handle<v8::Value> reflectedBooleanAttrAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.reflectedBooleanAttr._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8Boolean(imp->hasAttribute(WebCore::HTMLNames::reflectedbooleanattrAttr));
}