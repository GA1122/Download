static v8::Handle<v8::Value> conditionalMethod3Callback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.conditionalMethod3");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    imp->conditionalMethod3();
    return v8::Handle<v8::Value>();
}
