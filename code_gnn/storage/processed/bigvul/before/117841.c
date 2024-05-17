static v8::Handle<v8::Value> orangeCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.orange");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    imp->banana();
    return v8::Handle<v8::Value>();
}
