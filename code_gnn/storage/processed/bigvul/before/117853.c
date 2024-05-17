static v8::Handle<v8::Value> reflectedCustomURLAttrAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.reflectedCustomURLAttr._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8String(imp->getURLAttribute(WebCore::HTMLNames::customContentURLAttrAttr), info.GetIsolate());
}
