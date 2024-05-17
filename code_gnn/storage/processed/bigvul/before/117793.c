static v8::Handle<v8::Value> conditionalMethod1Callback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.conditionalMethod1");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    return v8String(imp->conditionalMethod1(), args.GetIsolate());
}
