static v8::Persistent<v8::FunctionTemplate> ConfigureV8Float64ArrayTemplate(v8::Persistent<v8::FunctionTemplate> desc)
{
    desc->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    defaultSignature = configureTemplate(desc, "Float64Array", V8ArrayBufferView::GetTemplate(), V8Float64Array::internalFieldCount,
        0, 0,
        0, 0);
    UNUSED_PARAM(defaultSignature);  
    desc->SetCallHandler(V8Float64Array::constructorCallback);
    v8::Local<v8::ObjectTemplate> instance = desc->InstanceTemplate();
    v8::Local<v8::ObjectTemplate> proto = desc->PrototypeTemplate();
    UNUSED_PARAM(instance);  
    UNUSED_PARAM(proto);  
    

    const int fooArgc = 1;
    v8::Handle<v8::FunctionTemplate> fooArgv[fooArgc] = { V8Float32Array::GetRawTemplate() };
    v8::Handle<v8::Signature> fooSignature = v8::Signature::New(desc, fooArgc, fooArgv);
    proto->Set(v8::String::New("foo"), v8::FunctionTemplate::New(Float64ArrayV8Internal::fooCallback, v8::Handle<v8::Value>(), fooSignature));

    desc->Set(getToStringName(), getToStringTemplate());
    return desc;
}
