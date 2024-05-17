static v8::Handle<v8::Value> XMLObjAttrAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.XMLObjAttr._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return toV8(imp->xmlObjAttr(), info.GetIsolate());
}
