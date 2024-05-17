static void configureV8TestObjectPythonTemplate(v8::Handle<v8::FunctionTemplate> functionTemplate, v8::Isolate* isolate)
{
    functionTemplate->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    defaultSignature = V8DOMConfiguration::installDOMClassTemplate(functionTemplate, "TestObjectPython", v8::Local<v8::FunctionTemplate>(), V8TestObjectPython::internalFieldCount,
        V8TestObjectPythonAttributes, WTF_ARRAY_LENGTH(V8TestObjectPythonAttributes),
        V8TestObjectPythonAccessors, WTF_ARRAY_LENGTH(V8TestObjectPythonAccessors),
        V8TestObjectPythonMethods, WTF_ARRAY_LENGTH(V8TestObjectPythonMethods),
        isolate);
    v8::Local<v8::ObjectTemplate> ALLOW_UNUSED instanceTemplate = functionTemplate->InstanceTemplate();
    v8::Local<v8::ObjectTemplate> ALLOW_UNUSED prototypeTemplate = functionTemplate->PrototypeTemplate();
    if (RuntimeEnabledFeatures::featureNameEnabled()) {
        static const V8DOMConfiguration::AttributeConfiguration attributeConfiguration =\
        {"runtimeEnabledLongAttribute", TestObjectPythonV8Internal::runtimeEnabledLongAttributeAttributeGetterCallback, TestObjectPythonV8Internal::runtimeEnabledLongAttributeAttributeSetterCallback, 0, 0, 0, static_cast<v8::AccessControl>(v8::DEFAULT), static_cast<v8::PropertyAttribute>(v8::None), 0  };
        V8DOMConfiguration::installAttribute(instanceTemplate, prototypeTemplate, attributeConfiguration, isolate);
    }
#if ENABLE(CONDITION)
    if (RuntimeEnabledFeatures::featureNameEnabled()) {
        static const V8DOMConfiguration::AttributeConfiguration attributeConfiguration =\
        {"conditionalRuntimeEnabledLongAttribute", TestObjectPythonV8Internal::conditionalRuntimeEnabledLongAttributeAttributeGetterCallback, TestObjectPythonV8Internal::conditionalRuntimeEnabledLongAttributeAttributeSetterCallback, 0, 0, 0, static_cast<v8::AccessControl>(v8::DEFAULT), static_cast<v8::PropertyAttribute>(v8::None), 0  };
        V8DOMConfiguration::installAttribute(instanceTemplate, prototypeTemplate, attributeConfiguration, isolate);
    }
#endif  
    static const V8DOMConfiguration::ConstantConfiguration V8TestObjectPythonConstants[] = {
        {"CONST_VALUE_0", 0},
        {"CONST_VALUE_1", 1},
        {"CONST_VALUE_2", 2},
        {"CONST_VALUE_4", 4},
        {"CONST_VALUE_8", 8},
        {"CONST_VALUE_9", -1},
        {"CONST_VALUE_10", "my constant string"},
        {"CONST_VALUE_11", 0xffffffff},
        {"CONST_VALUE_12", 0x01},
        {"CONST_VALUE_13", 0X20},
        {"CONST_VALUE_14", 0x1abc},
        {"CONST_VALUE_15", 010},
        {"CONST_VALUE_16", -010},
        {"CONST_VALUE_16", -0x1A},
        {"CONST_VALUE_17", -0X1a},
        {"DEPRECATED_CONSTANT", 1},
        {"CONST_JAVASCRIPT", 1},
    };
    V8DOMConfiguration::installConstants(functionTemplate, prototypeTemplate, V8TestObjectPythonConstants, WTF_ARRAY_LENGTH(V8TestObjectPythonConstants), isolate);
    if (RuntimeEnabledFeatures::featureNameEnabled()) {
        static const V8DOMConfiguration::ConstantConfiguration constantConfiguration = {"FEATURE_ENABLED_CONST", static_cast<signed int>(1)};
        V8DOMConfiguration::installConstants(functionTemplate, prototypeTemplate, &constantConfiguration, 1, isolate);
    }
    COMPILE_ASSERT(0 == TestObjectPython::CONST_VALUE_0, TheValueOfTestObjectPython_CONST_VALUE_0DoesntMatchWithImplementation);
    COMPILE_ASSERT(1 == TestObjectPython::CONST_VALUE_1, TheValueOfTestObjectPython_CONST_VALUE_1DoesntMatchWithImplementation);
    COMPILE_ASSERT(2 == TestObjectPython::CONST_VALUE_2, TheValueOfTestObjectPython_CONST_VALUE_2DoesntMatchWithImplementation);
    COMPILE_ASSERT(4 == TestObjectPython::CONST_VALUE_4, TheValueOfTestObjectPython_CONST_VALUE_4DoesntMatchWithImplementation);
    COMPILE_ASSERT(8 == TestObjectPython::CONST_VALUE_8, TheValueOfTestObjectPython_CONST_VALUE_8DoesntMatchWithImplementation);
    COMPILE_ASSERT(-1 == TestObjectPython::CONST_VALUE_9, TheValueOfTestObjectPython_CONST_VALUE_9DoesntMatchWithImplementation);
    COMPILE_ASSERT("my constant string" == TestObjectPython::CONST_VALUE_10, TheValueOfTestObjectPython_CONST_VALUE_10DoesntMatchWithImplementation);
    COMPILE_ASSERT(0xffffffff == TestObjectPython::CONST_VALUE_11, TheValueOfTestObjectPython_CONST_VALUE_11DoesntMatchWithImplementation);
    COMPILE_ASSERT(0x01 == TestObjectPython::CONST_VALUE_12, TheValueOfTestObjectPython_CONST_VALUE_12DoesntMatchWithImplementation);
    COMPILE_ASSERT(0X20 == TestObjectPython::CONST_VALUE_13, TheValueOfTestObjectPython_CONST_VALUE_13DoesntMatchWithImplementation);
    COMPILE_ASSERT(0x1abc == TestObjectPython::CONST_VALUE_14, TheValueOfTestObjectPython_CONST_VALUE_14DoesntMatchWithImplementation);
    COMPILE_ASSERT(010 == TestObjectPython::CONST_VALUE_15, TheValueOfTestObjectPython_CONST_VALUE_15DoesntMatchWithImplementation);
    COMPILE_ASSERT(-010 == TestObjectPython::CONST_VALUE_16, TheValueOfTestObjectPython_CONST_VALUE_16DoesntMatchWithImplementation);
    COMPILE_ASSERT(-0x1A == TestObjectPython::CONST_VALUE_16, TheValueOfTestObjectPython_CONST_VALUE_16DoesntMatchWithImplementation);
    COMPILE_ASSERT(-0X1a == TestObjectPython::CONST_VALUE_17, TheValueOfTestObjectPython_CONST_VALUE_17DoesntMatchWithImplementation);
    COMPILE_ASSERT(1 == TestObjectPython::DEPRECATED_CONSTANT, TheValueOfTestObjectPython_DEPRECATED_CONSTANTDoesntMatchWithImplementation);
    COMPILE_ASSERT(1 == TestObjectPython::FEATURE_ENABLED_CONST, TheValueOfTestObjectPython_FEATURE_ENABLED_CONSTDoesntMatchWithImplementation);
    COMPILE_ASSERT(1 == TestObjectPython::CONST_IMPL, TheValueOfTestObjectPython_CONST_IMPLDoesntMatchWithImplementation);
    functionTemplate->Set(v8AtomicString(isolate, "staticVoidMethod"), v8::FunctionTemplate::New(isolate, TestObjectPythonV8Internal::staticVoidMethodMethodCallback, v8Undefined(), v8::Local<v8::Signature>(), 0));
    functionTemplate->Set(v8AtomicString(isolate, "overloadedStaticMethod"), v8::FunctionTemplate::New(isolate, TestObjectPythonV8Internal::overloadedStaticMethodMethodCallback, v8Undefined(), v8::Local<v8::Signature>(), 1));
#if ENABLE(CONDITION)
    functionTemplate->Set(v8AtomicString(isolate, "conditionalConditionStaticVoidMethod"), v8::FunctionTemplate::New(isolate, TestObjectPythonV8Internal::conditionalConditionStaticVoidMethodMethodCallback, v8Undefined(), v8::Local<v8::Signature>(), 0));
#endif  
    prototypeTemplate->Set(v8AtomicString(isolate, "doNotCheckSignatureVoidMethod"), v8::FunctionTemplate::New(isolate, TestObjectPythonV8Internal::doNotCheckSignatureVoidMethodMethodCallback, v8Undefined(), v8::Local<v8::Signature>(), 0));
    prototypeTemplate->Set(v8AtomicString(isolate, "notEnumerableVoidMethod"), v8::FunctionTemplate::New(isolate, TestObjectPythonV8Internal::notEnumerableVoidMethodMethodCallback, v8Undefined(), defaultSignature, 0), static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::DontEnum));
    prototypeTemplate->Set(v8AtomicString(isolate, "readOnlyVoidMethod"), v8::FunctionTemplate::New(isolate, TestObjectPythonV8Internal::readOnlyVoidMethodMethodCallback, v8Undefined(), defaultSignature, 0), static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::ReadOnly));
    prototypeTemplate->Set(v8AtomicString(isolate, "notEnumerableReadOnlyVoidMethod"), v8::FunctionTemplate::New(isolate, TestObjectPythonV8Internal::notEnumerableReadOnlyVoidMethodMethodCallback, v8Undefined(), defaultSignature, 0), static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::DontEnum | v8::ReadOnly));
    if (RuntimeEnabledFeatures::featureNameEnabled())
        prototypeTemplate->Set(v8AtomicString(isolate, "runtimeEnabledVoidMethod"), v8::FunctionTemplate::New(isolate, TestObjectPythonV8Internal::runtimeEnabledVoidMethodMethodCallback, v8Undefined(), defaultSignature, 0));
    if (DOMWrapperWorld::current(isolate)->isMainWorld()) {
        if (RuntimeEnabledFeatures::featureNameEnabled())
            prototypeTemplate->Set(v8AtomicString(isolate, "perWorldBindingsRuntimeEnabledVoidMethod"), v8::FunctionTemplate::New(isolate, TestObjectPythonV8Internal::perWorldBindingsRuntimeEnabledVoidMethodMethodCallbackForMainWorld, v8Undefined(), defaultSignature, 0));
    } else {
        if (RuntimeEnabledFeatures::featureNameEnabled())
            prototypeTemplate->Set(v8AtomicString(isolate, "perWorldBindingsRuntimeEnabledVoidMethod"), v8::FunctionTemplate::New(isolate, TestObjectPythonV8Internal::perWorldBindingsRuntimeEnabledVoidMethodMethodCallback, v8Undefined(), defaultSignature, 0));
    }
    instanceTemplate->Set(v8AtomicString(isolate, "unforgeableVoidMethod"), v8::FunctionTemplate::New(isolate, TestObjectPythonV8Internal::unforgeableVoidMethodMethodCallback, v8Undefined(), defaultSignature, 0));
    functionTemplate->SetNativeDataProperty(v8AtomicString(isolate, "staticStringAttribute"), TestObjectPythonV8Internal::staticStringAttributeAttributeGetterCallback, TestObjectPythonV8Internal::staticStringAttributeAttributeSetterCallback, v8::External::New(isolate, 0), static_cast<v8::PropertyAttribute>(v8::None), v8::Handle<v8::AccessorSignature>(), static_cast<v8::AccessControl>(v8::DEFAULT));
    functionTemplate->SetNativeDataProperty(v8AtomicString(isolate, "staticLongAttribute"), TestObjectPythonV8Internal::staticLongAttributeAttributeGetterCallback, TestObjectPythonV8Internal::staticLongAttributeAttributeSetterCallback, v8::External::New(isolate, 0), static_cast<v8::PropertyAttribute>(v8::None), v8::Handle<v8::AccessorSignature>(), static_cast<v8::AccessControl>(v8::DEFAULT));

    functionTemplate->Set(v8AtomicString(isolate, "toString"), V8PerIsolateData::current()->toStringTemplate());
}
