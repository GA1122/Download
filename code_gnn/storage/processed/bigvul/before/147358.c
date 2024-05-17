static void DocumentTypeAttributeAttributeSetter(
    v8::Local<v8::Value> v8_value, const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  ALLOW_UNUSED_LOCAL(isolate);

  v8::Local<v8::Object> holder = info.Holder();
  ALLOW_UNUSED_LOCAL(holder);

  TestObject* impl = V8TestObject::ToImpl(holder);

  ExceptionState exception_state(isolate, ExceptionState::kSetterContext, "TestObject", "documentTypeAttribute");

  DocumentType* cpp_value = V8DocumentType::ToImplWithTypeCheck(info.GetIsolate(), v8_value);

  if (!cpp_value) {
    exception_state.ThrowTypeError("The provided value is not of type 'DocumentType'.");
    return;
  }

  impl->setDocumentTypeAttribute(cpp_value);
}
