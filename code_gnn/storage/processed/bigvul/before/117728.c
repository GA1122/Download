static v8::Persistent<v8::FunctionTemplate> ConfigureV8TestEventConstructorTemplate(v8::Persistent<v8::FunctionTemplate> desc)
{
    desc->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    defaultSignature = configureTemplate(desc, "TestEventConstructor", v8::Persistent<v8::FunctionTemplate>(), V8TestEventConstructor::internalFieldCount,
        TestEventConstructorAttrs, WTF_ARRAY_LENGTH(TestEventConstructorAttrs),
        0, 0);
    UNUSED_PARAM(defaultSignature);  
    desc->SetCallHandler(V8TestEventConstructor::constructorCallback);
    

    desc->Set(getToStringName(), getToStringTemplate());
    return desc;
}
