static void mutablePointAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.mutablePoint._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    RefPtr<SVGPropertyTearOff<FloatPoint> > v = V8SVGPoint::HasInstance(value) ? V8SVGPoint::toNative(v8::Handle<v8::Object>::Cast(value)) : 0;
    imp->setMutablePoint(WTF::getPtr(v));
    return;
}
