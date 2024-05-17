v8::Local<v8::FunctionTemplate> V8TestObject::DomTemplate(
    v8::Isolate* isolate, const DOMWrapperWorld& world) {
  return V8DOMConfiguration::DomClassTemplate(
      isolate, world, const_cast<WrapperTypeInfo*>(V8TestObject::GetWrapperTypeInfo()),
      InstallV8TestObjectTemplate);
}
