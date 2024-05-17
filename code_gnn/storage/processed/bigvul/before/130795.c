void V8TestObject::installPerContextEnabledProperties(v8::Handle<v8::Object> instanceTemplate, TestObject* impl, v8::Isolate* isolate)
{
    v8::Local<v8::Object> prototypeTemplate = v8::Local<v8::Object>::Cast(instanceTemplate->GetPrototype());
    if (ContextFeatures::featureNameEnabled(impl->document())) {
        static const V8DOMConfiguration::AttributeConfiguration attributeConfiguration =\
        {"enabledPerContextAttr", TestObjectV8Internal::enabledPerContextAttrAttributeGetterCallback, TestObjectV8Internal::enabledPerContextAttrAttributeSetterCallback, 0, 0, 0, static_cast<v8::AccessControl>(v8::DEFAULT), static_cast<v8::PropertyAttribute>(v8::None), 0  };
        V8DOMConfiguration::installAttribute(instanceTemplate, prototypeTemplate, attributeConfiguration, isolate);
    }
}
