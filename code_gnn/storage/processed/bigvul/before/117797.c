static void createAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.create._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    bool v = value->BooleanValue();
    imp->setCreate(v);
    return;
}
