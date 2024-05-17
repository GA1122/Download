void batchConfigureCallbacks(v8::Handle<v8::ObjectTemplate> proto, 
                             v8::Handle<v8::Signature> signature, 
                             v8::PropertyAttribute attributes,
                             const BatchedCallback* callbacks,
                             size_t callbackCount)
{
    for (size_t i = 0; i < callbackCount; ++i) {
        proto->Set(v8::String::New(callbacks[i].name),
                   v8::FunctionTemplate::New(callbacks[i].callback, 
                                             v8::Handle<v8::Value>(),
                                             signature),
                   attributes);
    }
}
