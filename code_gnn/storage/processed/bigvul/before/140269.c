v8::Handle<v8::Value> toV8(DOMWindow* window, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{

    if (!window)
        return v8::Null(isolate);
    Frame * frame = window->frame();
    if (!frame)
        return v8Undefined();

    v8::Handle<v8::Context> context = toV8Context(frame, DOMWrapperWorld::current(isolate));
    if (context.IsEmpty())
        return v8Undefined();

    v8::Handle<v8::Object> global = context->Global();
    ASSERT(!global.IsEmpty());
    return global;
}
