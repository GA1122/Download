static v8::Handle<v8::Value> objMethodCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.objMethod");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    return toV8(imp->objMethod(), args.GetIsolate());
}
