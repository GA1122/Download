static v8::Persistent<v8::FunctionTemplate> ConfigureV8TestSerializedScriptValueInterfaceTemplate(v8::Persistent<v8::FunctionTemplate> desc)
{
    desc->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    defaultSignature = configureTemplate(desc, "TestSerializedScriptValueInterface", v8::Persistent<v8::FunctionTemplate>(), V8TestSerializedScriptValueInterface::internalFieldCount,
        TestSerializedScriptValueInterfaceAttrs, WTF_ARRAY_LENGTH(TestSerializedScriptValueInterfaceAttrs),
        TestSerializedScriptValueInterfaceCallbacks, WTF_ARRAY_LENGTH(TestSerializedScriptValueInterfaceCallbacks));
    UNUSED_PARAM(defaultSignature);  
    desc->SetCallHandler(V8TestSerializedScriptValueInterface::constructorCallback);
    v8::Local<v8::ObjectTemplate> instance = desc->InstanceTemplate();
    v8::Local<v8::ObjectTemplate> proto = desc->PrototypeTemplate();
    UNUSED_PARAM(instance);  
    UNUSED_PARAM(proto);  
    

    desc->Set(getToStringName(), getToStringTemplate());
    return desc;
}
