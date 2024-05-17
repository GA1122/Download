static v8::Handle<v8::Value> cachedAttribute2AttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.cachedAttribute2._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    RefPtr<any> result = imp->cachedAttribute2();
    v8::Handle<v8::Value> wrapper = result.get() ? getDOMObjectMap(info.GetIsolate()).get(result.get()) : v8::Handle<v8::Object>();
    if (wrapper.IsEmpty()) {
        wrapper = toV8(result.get(), info.GetIsolate());
        if (!wrapper.IsEmpty())
            V8DOMWrapper::setNamedHiddenReference(info.Holder(), "cachedAttribute2", wrapper);
    }
    return wrapper;
}
