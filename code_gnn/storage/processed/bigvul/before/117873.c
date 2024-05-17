static void strictFloatAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.strictFloat._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    float v = static_cast<float>(value->NumberValue());
    imp->setStrictFloat(v);
    return;
}
