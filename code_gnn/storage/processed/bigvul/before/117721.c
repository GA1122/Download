v8::Handle<v8::Object> V8TestActiveDOMObject::wrapSlow(PassRefPtr<TestActiveDOMObject> impl, v8::Isolate* isolate)
{
    v8::Handle<v8::Object> wrapper;
    V8Proxy* proxy = 0;
    if (impl->frame()) {
        proxy = V8Proxy::retrieve(impl->frame());
        if (proxy)
            proxy->windowShell()->initContextIfNeeded();
    }

    v8::Handle<v8::Context> context;
    if (proxy && !proxy->matchesCurrentContext()) {
        context = proxy->context();
        if (!context.IsEmpty())
            context->Enter();
    }
    wrapper = V8DOMWrapper::instantiateV8Object(proxy, &info, impl.get());
    if (!context.IsEmpty())
        context->Exit();
    if (UNLIKELY(wrapper.IsEmpty()))
        return wrapper;

    v8::Persistent<v8::Object> wrapperHandle = v8::Persistent<v8::Object>::New(wrapper);

    if (!hasDependentLifetime)
        wrapperHandle.MarkIndependent();
    V8DOMWrapper::setJSWrapperForDOMObject(impl, wrapperHandle, isolate);
    return wrapper;
}
