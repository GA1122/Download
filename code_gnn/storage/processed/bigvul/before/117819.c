static v8::Handle<v8::Value> immutablePointFunctionCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.immutablePointFunction");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    return toV8(WTF::getPtr(SVGPropertyTearOff<FloatPoint>::create(imp->immutablePointFunction())), args.GetIsolate());
}
