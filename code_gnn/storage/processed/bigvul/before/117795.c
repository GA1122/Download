static v8::Handle<v8::Value> contentDocumentAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.contentDocument._get");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    if (!V8BindingSecurity::shouldAllowAccessToNode(V8BindingState::Only(), imp->contentDocument()))
        return v8::Handle<v8::Value>(v8::Null());

    return toV8(imp->contentDocument(), info.GetIsolate());
}
