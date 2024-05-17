static void enabledAtRuntimeAttr1AttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.enabledAtRuntimeAttr1._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    int v = toInt32(value);
    imp->setEnabledAtRuntimeAttr1(v);
    return;
}
