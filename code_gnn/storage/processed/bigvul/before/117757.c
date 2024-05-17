static v8::Persistent<v8::FunctionTemplate> ConfigureV8TestMediaQueryListListenerTemplate(v8::Persistent<v8::FunctionTemplate> desc)
{
    desc->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    defaultSignature = configureTemplate(desc, "TestMediaQueryListListener", v8::Persistent<v8::FunctionTemplate>(), V8TestMediaQueryListListener::internalFieldCount,
        0, 0,
        TestMediaQueryListListenerCallbacks, WTF_ARRAY_LENGTH(TestMediaQueryListListenerCallbacks));
    UNUSED_PARAM(defaultSignature);  
    v8::Local<v8::ObjectTemplate> instance = desc->InstanceTemplate();
    v8::Local<v8::ObjectTemplate> proto = desc->PrototypeTemplate();
    UNUSED_PARAM(instance);  
    UNUSED_PARAM(proto);  
    

    desc->Set(getToStringName(), getToStringTemplate());
    return desc;
}
