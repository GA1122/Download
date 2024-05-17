static v8::Local<v8::Object> wrapInShadowTemplate(v8::Local<v8::Object> wrapper, ScriptWrappable* scriptWrappable, v8::Isolate* isolate)
{
    static int shadowTemplateKey;  
    V8PerIsolateData* data = V8PerIsolateData::from(isolate);
    v8::Local<v8::FunctionTemplate> shadowTemplate = data->existingDOMTemplate(&shadowTemplateKey);
    if (shadowTemplate.IsEmpty()) {
        shadowTemplate = v8::FunctionTemplate::New(isolate);
        if (shadowTemplate.IsEmpty())
            return v8::Local<v8::Object>();
        shadowTemplate->SetClassName(v8AtomicString(isolate, "HTMLDocument"));
        shadowTemplate->Inherit(V8HTMLDocument::domTemplate(isolate));
        shadowTemplate->InstanceTemplate()->SetInternalFieldCount(V8HTMLDocument::internalFieldCount);
        data->setDOMTemplate(&shadowTemplateKey, shadowTemplate);
    }

    v8::Local<v8::Function> shadowConstructor;
    if (!shadowTemplate->GetFunction(isolate->GetCurrentContext()).ToLocal(&shadowConstructor))
        return v8::Local<v8::Object>();

    v8::Local<v8::Object> shadow;
    if (!V8ScriptRunner::instantiateObject(isolate, shadowConstructor).ToLocal(&shadow))
        return v8::Local<v8::Object>();
    if (!v8CallBoolean(shadow->SetPrototype(isolate->GetCurrentContext(), wrapper)))
        return v8::Local<v8::Object>();
    V8DOMWrapper::setNativeInfo(wrapper, &V8HTMLDocument::wrapperTypeInfo, scriptWrappable);
    return shadow;
}
