static void reflectedCustomURLAttrAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.reflectedCustomURLAttr._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    STRING_TO_V8PARAMETER_EXCEPTION_BLOCK_VOID(V8Parameter<WithNullCheck>, v, value);
    imp->setAttribute(WebCore::HTMLNames::customContentURLAttrAttr, v);
    return;
}