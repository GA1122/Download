static v8::Handle<v8::Value> supplementalNodeAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestInterface.supplementalNode._get");
    TestInterface* imp = V8TestInterface::toNative(info.Holder());
    return toV8(TestSupplemental::supplementalNode(imp), info.GetIsolate());
}
