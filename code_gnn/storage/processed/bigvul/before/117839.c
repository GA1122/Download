static v8::Handle<v8::Value> mutablePointFunctionCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.mutablePointFunction");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    return toV8(WTF::getPtr(SVGPropertyTearOff<FloatPoint>::create(imp->mutablePointFunction())), args.GetIsolate());
}
