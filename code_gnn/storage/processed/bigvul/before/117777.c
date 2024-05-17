static v8::Handle<v8::Value> addEventListenerCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.addEventListener()");
    RefPtr<EventListener> listener = V8DOMWrapper::getEventListener(args[1], false, ListenerFindOrCreate);
    if (listener) {
        V8TestObj::toNative(args.Holder())->addEventListener(v8ValueToAtomicWebCoreString(args[0]), listener, args[2]->BooleanValue());
        createHiddenDependency(args.Holder(), args[1], V8TestObj::eventListenerCacheIndex);
    }
    return v8::Undefined();
}
