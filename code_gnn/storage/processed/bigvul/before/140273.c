v8::Handle<v8::Object> toObjectWrapper(T* domObject, ScriptState* scriptState)
{
    if (!domObject)
        return v8::Object::New(scriptState->isolate());
    v8::Handle<v8::Value> value = toV8(domObject, scriptState->context()->Global(), scriptState->isolate());
    if (value.IsEmpty())
        return v8::Object::New(scriptState->isolate());
    return v8::Local<v8::Object>::New(scriptState->isolate(), value.As<v8::Object>());
}
