static v8::Handle<v8::Value> stringAttrWithSetterExceptionAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.stringAttrWithSetterException._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    return v8String(imp->stringAttrWithSetterException(), info.GetIsolate());
}
