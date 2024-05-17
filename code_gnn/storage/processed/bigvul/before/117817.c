static v8::Handle<v8::Value> immutablePointAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.immutablePoint._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return toV8(WTF::getPtr(SVGPropertyTearOff<FloatPoint>::create(imp->immutablePoint())), info.GetIsolate());
}
