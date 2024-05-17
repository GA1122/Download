static void conditionalAttr2AttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.conditionalAttr2._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    int v = toInt32(value);
    imp->setConditionalAttr2(v);
    return;
}
