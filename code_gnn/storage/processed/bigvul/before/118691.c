static void getter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    AtomicString name = toWebCoreAtomicString(property);
    HTMLDocument* htmlDocument = V8HTMLDocument::toNative(info.Holder());
    ASSERT(htmlDocument);
    v8::Handle<v8::Value> result = getNamedProperty(htmlDocument, name, info.Holder(), info.GetIsolate());
    if (!result.IsEmpty()) {
        v8SetReturnValue(info, result);
        return;
    }
    v8::Handle<v8::Value> prototype = info.Holder()->GetPrototype();
    if (prototype->IsObject()) {
        v8SetReturnValue(info, prototype.As<v8::Object>()->Get(property));
        return;
    }
}
