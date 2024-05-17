static v8::Handle<v8::ArrayBuffer> toV8Object(ArrayBuffer* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    if (!impl)
        return v8::Handle<v8::ArrayBuffer>();
    v8::Handle<v8::Value> wrapper = toV8(impl, creationContext, isolate);
    ASSERT(wrapper->IsArrayBuffer());
    return wrapper.As<v8::ArrayBuffer>();
}
