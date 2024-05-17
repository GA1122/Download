static v8::Handle<v8::Value> mutablePointAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.mutablePoint._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return toV8(WTF::getPtr(SVGStaticPropertyTearOff<TestObj, FloatPoint>::create(imp, imp->mutablePoint(), &TestObj::updateMutablePoint)), info.GetIsolate());
}
