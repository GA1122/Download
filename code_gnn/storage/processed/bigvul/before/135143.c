v8::Local<v8::Object> Document::associateWithWrapper(v8::Isolate* isolate, const WrapperTypeInfo* wrapperType, v8::Local<v8::Object> wrapper)
{
    wrapper = V8DOMWrapper::associateObjectWithWrapper(isolate, this, wrapperType, wrapper);
    DOMWrapperWorld& world = DOMWrapperWorld::current(isolate);
    if (world.isMainWorld() && frame())
        frame()->script().windowProxy(world)->updateDocumentWrapper(wrapper);
    return wrapper;
}
