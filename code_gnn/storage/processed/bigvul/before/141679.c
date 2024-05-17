v8::Local<v8::Private> inspectedContextPrivateKey(v8::Isolate* isolate)
{
    return v8::Private::ForApi(isolate, toV8StringInternalized(isolate, "V8Console#InspectedContext"));
}
