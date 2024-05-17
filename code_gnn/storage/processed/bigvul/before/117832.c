static v8::Handle<v8::Value> methodWithExceptionCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.methodWithException");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    ExceptionCode ec = 0;
    {
    imp->methodWithException(ec);
    if (UNLIKELY(ec))
        goto fail;
    return v8::Handle<v8::Value>();
    }
    fail:
    V8Proxy::setDOMException(ec, args.GetIsolate());
    return v8::Handle<v8::Value>();
}
