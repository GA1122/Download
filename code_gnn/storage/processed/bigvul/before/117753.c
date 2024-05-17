static v8::Handle<v8::Value> supplementalStr2AttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestInterface.supplementalStr2._get");
    TestInterface* imp = V8TestInterface::toNative(info.Holder());
    return v8String(TestSupplemental::supplementalStr2(imp), info.GetIsolate());
}
