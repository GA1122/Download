static v8::Persistent<v8::FunctionTemplate> ConfigureV8TestEventTargetTemplate(v8::Persistent<v8::FunctionTemplate> desc)
{
    desc->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    defaultSignature = configureTemplate(desc, "TestEventTarget", v8::Persistent<v8::FunctionTemplate>(), V8TestEventTarget::internalFieldCount,
        0, 0,
        TestEventTargetCallbacks, WTF_ARRAY_LENGTH(TestEventTargetCallbacks));
    UNUSED_PARAM(defaultSignature);  
    v8::Local<v8::ObjectTemplate> instance = desc->InstanceTemplate();
    v8::Local<v8::ObjectTemplate> proto = desc->PrototypeTemplate();
    UNUSED_PARAM(instance);  
    UNUSED_PARAM(proto);  
    
    setCollectionIndexedGetter<TestEventTarget, Node>(desc);
    desc->InstanceTemplate()->SetNamedPropertyHandler(V8TestEventTarget::namedPropertyGetter, 0, 0, 0, 0);
    desc->InstanceTemplate()->MarkAsUndetectable();

    const int dispatchEventArgc = 1;
    v8::Handle<v8::FunctionTemplate> dispatchEventArgv[dispatchEventArgc] = { V8Event::GetRawTemplate() };
    v8::Handle<v8::Signature> dispatchEventSignature = v8::Signature::New(desc, dispatchEventArgc, dispatchEventArgv);
    proto->Set(v8::String::New("dispatchEvent"), v8::FunctionTemplate::New(TestEventTargetV8Internal::dispatchEventCallback, v8::Handle<v8::Value>(), dispatchEventSignature));

    desc->Set(getToStringName(), getToStringTemplate());
    return desc;
}
