static v8::Handle<v8::Value> getSVGDocumentCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.getSVGDocument");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    ExceptionCode ec = 0;
    {
    if (!V8BindingSecurity::shouldAllowAccessToNode(V8BindingState::Only(), imp->getSVGDocument(ec)))
        return v8::Handle<v8::Value>(v8::Null());
    RefPtr<SVGDocument> result = imp->getSVGDocument(ec);
    if (UNLIKELY(ec))
        goto fail;
    return toV8(result.release(), args.GetIsolate());
    }
    fail:
    V8Proxy::setDOMException(ec, args.GetIsolate());
    return v8::Handle<v8::Value>();
}
