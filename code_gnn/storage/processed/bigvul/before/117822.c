static v8::Handle<v8::Value> intMethodCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.intMethod");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    return v8::Integer::New(imp->intMethod());
}
