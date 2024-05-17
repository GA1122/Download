static v8::Handle<v8::Value> attrWithGetterExceptionAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.attrWithGetterException._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    ExceptionCode ec = 0;
    int v = imp->attrWithGetterException(ec);
    if (UNLIKELY(ec)) {
        V8Proxy::setDOMException(ec, info.GetIsolate());
        return v8::Handle<v8::Value>();
    }
    return v8::Integer::New(v);
}
