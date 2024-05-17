static v8::Handle<v8::Value> supplementalStr1AttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestInterface.supplementalStr1._get");
    TestInterface* imp = V8TestInterface::toNative(info.Holder());
    return v8String(TestSupplemental::supplementalStr1(imp), info.GetIsolate());
}
