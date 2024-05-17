static v8::Handle<v8::Value> supplementalMethod1Callback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestInterface.supplementalMethod1");
    TestInterface* imp = V8TestInterface::toNative(args.Holder());
    TestSupplemental::supplementalMethod1(imp);
    return v8::Handle<v8::Value>();
}
