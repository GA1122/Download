v8::Handle<v8::Value> toV8(DataView* impl, v8::Isolate* isolate)
{
    if (!impl)
        return v8::Null();
    return V8DataView::wrap(impl, isolate);
}
