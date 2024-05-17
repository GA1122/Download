static void immutablePointAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.immutablePoint._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    RefPtr<SVGPropertyTearOff<FloatPoint> > v = V8SVGPoint::HasInstance(value) ? V8SVGPoint::toNative(v8::Handle<v8::Object>::Cast(value)) : 0;
    imp->setImmutablePoint(WTF::getPtr(v));
    return;
}
