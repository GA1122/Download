void V8TestObjectPython::installPerContextEnabledProperties(v8::Handle<v8::Object> instanceTemplate, TestObjectPython* impl, v8::Isolate* isolate)
{
    v8::Local<v8::Object> prototypeTemplate = v8::Local<v8::Object>::Cast(instanceTemplate->GetPrototype());
    if (ContextFeatures::featureNameEnabled(impl->document())) {
        static const V8DOMConfiguration::AttributeConfiguration attributeConfiguration =\
        {"perContextEnabledLongAttribute", TestObjectPythonV8Internal::perContextEnabledLongAttributeAttributeGetterCallback, TestObjectPythonV8Internal::perContextEnabledLongAttributeAttributeSetterCallback, 0, 0, 0, static_cast<v8::AccessControl>(v8::DEFAULT), static_cast<v8::PropertyAttribute>(v8::None), 0  };
        V8DOMConfiguration::installAttribute(instanceTemplate, prototypeTemplate, attributeConfiguration, isolate);
    }
    if (ContextFeatures::featureNameEnabled(impl->document())) {
        static const V8DOMConfiguration::AttributeConfiguration attributeConfiguration =\
        {"perContextEnabledRuntimeEnabledLongAttribute", TestObjectPythonV8Internal::perContextEnabledRuntimeEnabledLongAttributeAttributeGetterCallback, TestObjectPythonV8Internal::perContextEnabledRuntimeEnabledLongAttributeAttributeSetterCallback, 0, 0, 0, static_cast<v8::AccessControl>(v8::DEFAULT), static_cast<v8::PropertyAttribute>(v8::None), 0  };
        V8DOMConfiguration::installAttribute(instanceTemplate, prototypeTemplate, attributeConfiguration, isolate);
    }
}
