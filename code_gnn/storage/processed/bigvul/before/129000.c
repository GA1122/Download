static v8::Handle<v8::Object> toV8Object(MessagePort* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    if (!impl)
        return v8::Handle<v8::Object>();
    v8::Handle<v8::Value> wrapper = toV8(impl, creationContext, isolate);
    ASSERT(wrapper->IsObject());
    return wrapper.As<v8::Object>();
}
