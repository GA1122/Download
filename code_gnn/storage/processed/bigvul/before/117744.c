static v8::Persistent<v8::FunctionTemplate> ConfigureV8TestInterfaceTemplate(v8::Persistent<v8::FunctionTemplate> desc)
{
    desc->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    defaultSignature = configureTemplate(desc, "TestInterface", v8::Persistent<v8::FunctionTemplate>(), V8TestInterface::internalFieldCount,
        TestInterfaceAttrs, WTF_ARRAY_LENGTH(TestInterfaceAttrs),
        TestInterfaceCallbacks, WTF_ARRAY_LENGTH(TestInterfaceCallbacks));
    UNUSED_PARAM(defaultSignature);  
    desc->SetCallHandler(V8TestInterface::constructorCallback);
    v8::Local<v8::ObjectTemplate> instance = desc->InstanceTemplate();
    v8::Local<v8::ObjectTemplate> proto = desc->PrototypeTemplate();
    UNUSED_PARAM(instance);  
    UNUSED_PARAM(proto);  
    

    const int supplementalMethod2Argc = 2;
    v8::Handle<v8::FunctionTemplate> supplementalMethod2Argv[supplementalMethod2Argc] = { v8::Handle<v8::FunctionTemplate>(), V8TestObj::GetRawTemplate() };
    v8::Handle<v8::Signature> supplementalMethod2Signature = v8::Signature::New(desc, supplementalMethod2Argc, supplementalMethod2Argv);
#if ENABLE(Condition11) || ENABLE(Condition12)
    proto->Set(v8::String::New("supplementalMethod2"), v8::FunctionTemplate::New(TestInterfaceV8Internal::supplementalMethod2Callback, v8::Handle<v8::Value>(), supplementalMethod2Signature));
#endif  
#if ENABLE(Condition11) || ENABLE(Condition12)
    desc->Set(v8::String::New("supplementalMethod4"), v8::FunctionTemplate::New(TestInterfaceV8Internal::supplementalMethod4Callback, v8::Handle<v8::Value>(), v8::Local<v8::Signature>()));
#endif  
    batchConfigureConstants(desc, proto, TestInterfaceConsts, WTF_ARRAY_LENGTH(TestInterfaceConsts));

    desc->Set(getToStringName(), getToStringTemplate());
    return desc;
}
