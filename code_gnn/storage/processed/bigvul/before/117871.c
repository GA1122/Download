static void strawberryAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.strawberry._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    int v = V8int::HasInstance(value) ? V8int::toNative(v8::Handle<v8::Object>::Cast(value)) : 0;
    imp->setBlueberry(v);
    return;
}
