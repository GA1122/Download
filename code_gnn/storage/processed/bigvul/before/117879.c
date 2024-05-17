static v8::Handle<v8::Value> testObjAttrAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.testObjAttr._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return toV8(imp->testObjAttr(), info.GetIsolate());
}
