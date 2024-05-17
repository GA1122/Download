v8::Persistent<v8::FunctionTemplate> V8TestNamedConstructorConstructor::GetTemplate()
{
    static v8::Persistent<v8::FunctionTemplate> cachedTemplate;
    if (!cachedTemplate.IsEmpty())
        return cachedTemplate;

    v8::HandleScope scope;
    v8::Local<v8::FunctionTemplate> result = v8::FunctionTemplate::New(V8TestNamedConstructorConstructorCallback);

    v8::Local<v8::ObjectTemplate> instance = result->InstanceTemplate();
    instance->SetInternalFieldCount(V8TestNamedConstructor::internalFieldCount);
    result->SetClassName(v8::String::New("TestNamedConstructor"));
    result->Inherit(V8TestNamedConstructor::GetTemplate());

    cachedTemplate = v8::Persistent<v8::FunctionTemplate>::New(result);
    return cachedTemplate;
}
