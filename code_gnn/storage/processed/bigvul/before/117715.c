static v8::Persistent<v8::FunctionTemplate> ConfigureV8TestActiveDOMObjectTemplate(v8::Persistent<v8::FunctionTemplate> desc)
{
    desc->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    defaultSignature = configureTemplate(desc, "TestActiveDOMObject", v8::Persistent<v8::FunctionTemplate>(), V8TestActiveDOMObject::internalFieldCount,
        TestActiveDOMObjectAttrs, WTF_ARRAY_LENGTH(TestActiveDOMObjectAttrs),
        0, 0);
    UNUSED_PARAM(defaultSignature);  
    v8::Local<v8::ObjectTemplate> instance = desc->InstanceTemplate();
    v8::Local<v8::ObjectTemplate> proto = desc->PrototypeTemplate();
    UNUSED_PARAM(instance);  
    UNUSED_PARAM(proto);  
    instance->SetAccessCheckCallbacks(V8TestActiveDOMObject::namedSecurityCheck, V8TestActiveDOMObject::indexedSecurityCheck, v8::External::Wrap(&V8TestActiveDOMObject::info));

    const int excitingFunctionArgc = 1;
    v8::Handle<v8::FunctionTemplate> excitingFunctionArgv[excitingFunctionArgc] = { V8Node::GetRawTemplate() };
    v8::Handle<v8::Signature> excitingFunctionSignature = v8::Signature::New(desc, excitingFunctionArgc, excitingFunctionArgv);
    proto->Set(v8::String::New("excitingFunction"), v8::FunctionTemplate::New(TestActiveDOMObjectV8Internal::excitingFunctionCallback, v8::Handle<v8::Value>(), excitingFunctionSignature));

    proto->SetAccessor(v8::String::New("postMessage"), TestActiveDOMObjectV8Internal::postMessageAttrGetter, 0, v8::Handle<v8::Value>(), v8::ALL_CAN_READ, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::ReadOnly));

    desc->Set(getToStringName(), getToStringTemplate());
    return desc;
}
