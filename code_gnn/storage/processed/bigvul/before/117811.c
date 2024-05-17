static void floatArrayAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.floatArray._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    Vector<float> v = v8NumberArrayToVector<float>(value);
    imp->setFloatArray(v);
    return;
}
