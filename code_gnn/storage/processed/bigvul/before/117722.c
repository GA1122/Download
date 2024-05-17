static v8::Persistent<v8::FunctionTemplate> ConfigureV8TestCustomNamedGetterTemplate(v8::Persistent<v8::FunctionTemplate> desc)
{
    desc->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    defaultSignature = configureTemplate(desc, "TestCustomNamedGetter", v8::Persistent<v8::FunctionTemplate>(), V8TestCustomNamedGetter::internalFieldCount,
        0, 0,
        TestCustomNamedGetterCallbacks, WTF_ARRAY_LENGTH(TestCustomNamedGetterCallbacks));
    UNUSED_PARAM(defaultSignature);  
    v8::Local<v8::ObjectTemplate> instance = desc->InstanceTemplate();
    v8::Local<v8::ObjectTemplate> proto = desc->PrototypeTemplate();
    UNUSED_PARAM(instance);  
    UNUSED_PARAM(proto);  
    
    desc->InstanceTemplate()->SetNamedPropertyHandler(V8TestCustomNamedGetter::namedPropertyGetter, 0, 0, 0, 0);

    desc->Set(getToStringName(), getToStringTemplate());
    return desc;
}
