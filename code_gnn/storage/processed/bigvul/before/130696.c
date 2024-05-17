static void configureV8TestObjectTemplate(v8::Handle<v8::FunctionTemplate> functionTemplate, v8::Isolate* isolate)
{
    functionTemplate->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    defaultSignature = V8DOMConfiguration::installDOMClassTemplate(functionTemplate, "TestObject", V8EventTarget::domTemplate(isolate), V8TestObject::internalFieldCount,
        V8TestObjectAttributes, WTF_ARRAY_LENGTH(V8TestObjectAttributes),
        V8TestObjectAccessors, WTF_ARRAY_LENGTH(V8TestObjectAccessors),
        V8TestObjectMethods, WTF_ARRAY_LENGTH(V8TestObjectMethods),
        isolate);
    v8::Local<v8::ObjectTemplate> ALLOW_UNUSED instanceTemplate = functionTemplate->InstanceTemplate();
    v8::Local<v8::ObjectTemplate> ALLOW_UNUSED prototypeTemplate = functionTemplate->PrototypeTemplate();
    if (RuntimeEnabledFeatures::featureNameEnabled()) {
        static const V8DOMConfiguration::AttributeConfiguration attributeConfiguration =\
        {"enabledAtRuntimeAttr", TestObjectV8Internal::enabledAtRuntimeAttrAttributeGetterCallback, TestObjectV8Internal::enabledAtRuntimeAttrAttributeSetterCallback, 0, 0, 0, static_cast<v8::AccessControl>(v8::DEFAULT), static_cast<v8::PropertyAttribute>(v8::None), 0  };
        V8DOMConfiguration::installAttribute(instanceTemplate, prototypeTemplate, attributeConfiguration, isolate);
    }
    static const V8DOMConfiguration::ConstantConfiguration V8TestObjectConstants[] = {
        {"DEPRECATED_CONSTANT", 1},
    };
    V8DOMConfiguration::installConstants(functionTemplate, prototypeTemplate, V8TestObjectConstants, WTF_ARRAY_LENGTH(V8TestObjectConstants), isolate);
    COMPILE_ASSERT(1 == TestObject::DEPRECATED_CONSTANT, TheValueOfTestObject_DEPRECATED_CONSTANTDoesntMatchWithImplementation);
    functionTemplate->InstanceTemplate()->SetIndexedPropertyHandler(TestObjectV8Internal::indexedPropertyGetterCallback, 0, 0, 0, indexedPropertyEnumerator<TestObject>);
    functionTemplate->InstanceTemplate()->SetNamedPropertyHandler(TestObjectV8Internal::namedPropertyGetterCallback, 0, TestObjectV8Internal::namedPropertyQueryCallback, 0, TestObjectV8Internal::namedPropertyEnumeratorCallback);
    functionTemplate->Set(v8AtomicString(isolate, "staticMethodWithCallbackAndOptionalArg"), v8::FunctionTemplate::New(isolate, TestObjectV8Internal::staticMethodWithCallbackAndOptionalArgMethodCallback, v8Undefined(), v8::Local<v8::Signature>(), 0));
    functionTemplate->Set(v8AtomicString(isolate, "staticMethodWithCallbackArg"), v8::FunctionTemplate::New(isolate, TestObjectV8Internal::staticMethodWithCallbackArgMethodCallback, v8Undefined(), v8::Local<v8::Signature>(), 1));
    functionTemplate->Set(v8AtomicString(isolate, "classMethod"), v8::FunctionTemplate::New(isolate, TestObjectV8Internal::classMethodMethodCallback, v8Undefined(), v8::Local<v8::Signature>(), 0));
    functionTemplate->Set(v8AtomicString(isolate, "classMethodWithOptional"), v8::FunctionTemplate::New(isolate, TestObjectV8Internal::classMethodWithOptionalMethodCallback, v8Undefined(), v8::Local<v8::Signature>(), 0));
    functionTemplate->Set(v8AtomicString(isolate, "classMethod2"), v8::FunctionTemplate::New(isolate, TestObjectV8Internal::classMethod2MethodCallback, v8Undefined(), v8::Local<v8::Signature>(), 1));
    if (RuntimeEnabledFeatures::featureNameEnabled())
        prototypeTemplate->Set(v8AtomicString(isolate, "enabledAtRuntimeMethod"), v8::FunctionTemplate::New(isolate, TestObjectV8Internal::enabledAtRuntimeMethodMethodCallback, v8Undefined(), defaultSignature, 1));
    functionTemplate->Set(v8AtomicString(isolate, "deprecatedStaticMethod"), v8::FunctionTemplate::New(isolate, TestObjectV8Internal::deprecatedStaticMethodMethodCallback, v8Undefined(), v8::Local<v8::Signature>(), 0));
    functionTemplate->SetNativeDataProperty(v8AtomicString(isolate, "staticReadOnlyLongAttr"), TestObjectV8Internal::staticReadOnlyLongAttrAttributeGetterCallback, 0, v8::External::New(isolate, 0), static_cast<v8::PropertyAttribute>(v8::None), v8::Handle<v8::AccessorSignature>(), static_cast<v8::AccessControl>(v8::DEFAULT));
    functionTemplate->SetNativeDataProperty(v8AtomicString(isolate, "staticStringAttr"), TestObjectV8Internal::staticStringAttrAttributeGetterCallback, TestObjectV8Internal::staticStringAttrAttributeSetterCallback, v8::External::New(isolate, 0), static_cast<v8::PropertyAttribute>(v8::None), v8::Handle<v8::AccessorSignature>(), static_cast<v8::AccessControl>(v8::DEFAULT));
    functionTemplate->SetNativeDataProperty(v8AtomicString(isolate, "deprecatedStaticReadOnlyAttr"), TestObjectV8Internal::deprecatedStaticReadOnlyAttrAttributeGetterCallback, 0, v8::External::New(isolate, 0), static_cast<v8::PropertyAttribute>(v8::None), v8::Handle<v8::AccessorSignature>(), static_cast<v8::AccessControl>(v8::DEFAULT));
    functionTemplate->SetNativeDataProperty(v8AtomicString(isolate, "deprecatedStaticAttr"), TestObjectV8Internal::deprecatedStaticAttrAttributeGetterCallback, TestObjectV8Internal::deprecatedStaticAttrAttributeSetterCallback, v8::External::New(isolate, 0), static_cast<v8::PropertyAttribute>(v8::None), v8::Handle<v8::AccessorSignature>(), static_cast<v8::AccessControl>(v8::DEFAULT));

    functionTemplate->Set(v8AtomicString(isolate, "toString"), V8PerIsolateData::current()->toStringTemplate());
}
