static bool isDocumentType(v8::Handle<v8::Value> value)
{
    return V8Document::HasInstance(value) || V8HTMLDocument::HasInstance(value);
}
