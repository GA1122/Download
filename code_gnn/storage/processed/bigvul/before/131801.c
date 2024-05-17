v8::Handle<v8::Value> toV8NoInline(TestObjectPython* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    return toV8(impl, creationContext, isolate);
}
