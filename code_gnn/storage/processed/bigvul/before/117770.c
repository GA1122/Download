static v8::Persistent<v8::FunctionTemplate> ConfigureV8TestObjTemplate(v8::Persistent<v8::FunctionTemplate> desc)
{
    desc->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    defaultSignature = configureTemplate(desc, "TestObject", v8::Persistent<v8::FunctionTemplate>(), V8TestObj::internalFieldCount,
        TestObjAttrs, WTF_ARRAY_LENGTH(TestObjAttrs),
        TestObjCallbacks, WTF_ARRAY_LENGTH(TestObjCallbacks));
    UNUSED_PARAM(defaultSignature);  
    desc->SetCallHandler(V8TestObj::constructorCallback);
    v8::Local<v8::ObjectTemplate> instance = desc->InstanceTemplate();
    v8::Local<v8::ObjectTemplate> proto = desc->PrototypeTemplate();
    UNUSED_PARAM(instance);  
    UNUSED_PARAM(proto);  
    
    if (RuntimeEnabledFeatures::enabledAtRuntimeAttr1Enabled()) {
        static const BatchedAttribute attrData =\
        {"enabledAtRuntimeAttr1", TestObjV8Internal::enabledAtRuntimeAttr1AttrGetter, TestObjV8Internal::enabledAtRuntimeAttr1AttrSetter, 0  , static_cast<v8::AccessControl>(v8::DEFAULT), static_cast<v8::PropertyAttribute>(v8::None), 0  };
        configureAttribute(instance, proto, attrData);
    }
    if (RuntimeEnabledFeatures::featureNameEnabled()) {
        static const BatchedAttribute attrData =\
        {"enabledAtRuntimeAttr2", TestObjV8Internal::enabledAtRuntimeAttr2AttrGetter, TestObjV8Internal::enabledAtRuntimeAttr2AttrSetter, 0  , static_cast<v8::AccessControl>(v8::DEFAULT), static_cast<v8::PropertyAttribute>(v8::None), 0  };
        configureAttribute(instance, proto, attrData);
    }

    const int voidMethodWithArgsArgc = 3;
    v8::Handle<v8::FunctionTemplate> voidMethodWithArgsArgv[voidMethodWithArgsArgc] = { v8::Handle<v8::FunctionTemplate>(), v8::Handle<v8::FunctionTemplate>(), V8TestObj::GetRawTemplate() };
    v8::Handle<v8::Signature> voidMethodWithArgsSignature = v8::Signature::New(desc, voidMethodWithArgsArgc, voidMethodWithArgsArgv);
    proto->Set(v8::String::New("voidMethodWithArgs"), v8::FunctionTemplate::New(TestObjV8Internal::voidMethodWithArgsCallback, v8::Handle<v8::Value>(), voidMethodWithArgsSignature));

    const int intMethodWithArgsArgc = 3;
    v8::Handle<v8::FunctionTemplate> intMethodWithArgsArgv[intMethodWithArgsArgc] = { v8::Handle<v8::FunctionTemplate>(), v8::Handle<v8::FunctionTemplate>(), V8TestObj::GetRawTemplate() };
    v8::Handle<v8::Signature> intMethodWithArgsSignature = v8::Signature::New(desc, intMethodWithArgsArgc, intMethodWithArgsArgv);
    proto->Set(v8::String::New("intMethodWithArgs"), v8::FunctionTemplate::New(TestObjV8Internal::intMethodWithArgsCallback, v8::Handle<v8::Value>(), intMethodWithArgsSignature));

    const int objMethodWithArgsArgc = 3;
    v8::Handle<v8::FunctionTemplate> objMethodWithArgsArgv[objMethodWithArgsArgc] = { v8::Handle<v8::FunctionTemplate>(), v8::Handle<v8::FunctionTemplate>(), V8TestObj::GetRawTemplate() };
    v8::Handle<v8::Signature> objMethodWithArgsSignature = v8::Signature::New(desc, objMethodWithArgsArgc, objMethodWithArgsArgv);
    proto->Set(v8::String::New("objMethodWithArgs"), v8::FunctionTemplate::New(TestObjV8Internal::objMethodWithArgsCallback, v8::Handle<v8::Value>(), objMethodWithArgsSignature));

    const int methodWithSequenceArgArgc = 1;
    v8::Handle<v8::FunctionTemplate> methodWithSequenceArgArgv[methodWithSequenceArgArgc] = { V8sequence<ScriptProfile>::GetRawTemplate() };
    v8::Handle<v8::Signature> methodWithSequenceArgSignature = v8::Signature::New(desc, methodWithSequenceArgArgc, methodWithSequenceArgArgv);
    proto->Set(v8::String::New("methodWithSequenceArg"), v8::FunctionTemplate::New(TestObjV8Internal::methodWithSequenceArgCallback, v8::Handle<v8::Value>(), methodWithSequenceArgSignature));

    const int methodThatRequiresAllArgsAndThrowsArgc = 2;
    v8::Handle<v8::FunctionTemplate> methodThatRequiresAllArgsAndThrowsArgv[methodThatRequiresAllArgsAndThrowsArgc] = { v8::Handle<v8::FunctionTemplate>(), V8TestObj::GetRawTemplate() };
    v8::Handle<v8::Signature> methodThatRequiresAllArgsAndThrowsSignature = v8::Signature::New(desc, methodThatRequiresAllArgsAndThrowsArgc, methodThatRequiresAllArgsAndThrowsArgv);
    proto->Set(v8::String::New("methodThatRequiresAllArgsAndThrows"), v8::FunctionTemplate::New(TestObjV8Internal::methodThatRequiresAllArgsAndThrowsCallback, v8::Handle<v8::Value>(), methodThatRequiresAllArgsAndThrowsSignature));
    desc->Set(v8::String::New("classMethod"), v8::FunctionTemplate::New(TestObjV8Internal::classMethodCallback, v8::Handle<v8::Value>(), v8::Local<v8::Signature>()));
    desc->Set(v8::String::New("classMethodWithOptional"), v8::FunctionTemplate::New(TestObjV8Internal::classMethodWithOptionalCallback, v8::Handle<v8::Value>(), v8::Local<v8::Signature>()));
    desc->Set(v8::String::New("classMethod2"), v8::FunctionTemplate::New(V8TestObj::classMethod2Callback, v8::Handle<v8::Value>(), v8::Local<v8::Signature>()));
#if ENABLE(Condition1)
    desc->Set(v8::String::New("overloadedMethod1"), v8::FunctionTemplate::New(TestObjV8Internal::overloadedMethod1Callback, v8::Handle<v8::Value>(), v8::Local<v8::Signature>()));
#endif  
    if (RuntimeEnabledFeatures::enabledAtRuntimeMethod1Enabled())
        proto->Set(v8::String::New("enabledAtRuntimeMethod1"), v8::FunctionTemplate::New(TestObjV8Internal::enabledAtRuntimeMethod1Callback, v8::Handle<v8::Value>(), defaultSignature));
    if (RuntimeEnabledFeatures::featureNameEnabled())
        proto->Set(v8::String::New("enabledAtRuntimeMethod2"), v8::FunctionTemplate::New(TestObjV8Internal::enabledAtRuntimeMethod2Callback, v8::Handle<v8::Value>(), defaultSignature));

    const int convert1Argc = 1;
    v8::Handle<v8::FunctionTemplate> convert1Argv[convert1Argc] = { V8a::GetRawTemplate() };
    v8::Handle<v8::Signature> convert1Signature = v8::Signature::New(desc, convert1Argc, convert1Argv);
    proto->Set(v8::String::New("convert1"), v8::FunctionTemplate::New(TestObjV8Internal::convert1Callback, v8::Handle<v8::Value>(), convert1Signature));

    const int convert2Argc = 1;
    v8::Handle<v8::FunctionTemplate> convert2Argv[convert2Argc] = { V8b::GetRawTemplate() };
    v8::Handle<v8::Signature> convert2Signature = v8::Signature::New(desc, convert2Argc, convert2Argv);
    proto->Set(v8::String::New("convert2"), v8::FunctionTemplate::New(TestObjV8Internal::convert2Callback, v8::Handle<v8::Value>(), convert2Signature));

    const int convert3Argc = 1;
    v8::Handle<v8::FunctionTemplate> convert3Argv[convert3Argc] = { V8c::GetRawTemplate() };
    v8::Handle<v8::Signature> convert3Signature = v8::Signature::New(desc, convert3Argc, convert3Argv);
    proto->Set(v8::String::New("convert3"), v8::FunctionTemplate::New(TestObjV8Internal::convert3Callback, v8::Handle<v8::Value>(), convert3Signature));

    const int convert4Argc = 1;
    v8::Handle<v8::FunctionTemplate> convert4Argv[convert4Argc] = { V8d::GetRawTemplate() };
    v8::Handle<v8::Signature> convert4Signature = v8::Signature::New(desc, convert4Argc, convert4Argv);
    proto->Set(v8::String::New("convert4"), v8::FunctionTemplate::New(TestObjV8Internal::convert4Callback, v8::Handle<v8::Value>(), convert4Signature));

    const int convert5Argc = 1;
    v8::Handle<v8::FunctionTemplate> convert5Argv[convert5Argc] = { V8e::GetRawTemplate() };
    v8::Handle<v8::Signature> convert5Signature = v8::Signature::New(desc, convert5Argc, convert5Argv);
    proto->Set(v8::String::New("convert5"), v8::FunctionTemplate::New(TestObjV8Internal::convert5Callback, v8::Handle<v8::Value>(), convert5Signature));
    batchConfigureConstants(desc, proto, TestObjConsts, WTF_ARRAY_LENGTH(TestObjConsts));

    desc->Set(getToStringName(), getToStringTemplate());
    return desc;
}
