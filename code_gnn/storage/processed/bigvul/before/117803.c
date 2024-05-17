static void doubleArrayAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.doubleArray._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    Vector<double> v = v8NumberArrayToVector<double>(value);
    imp->setDoubleArray(v);
    return;
}
