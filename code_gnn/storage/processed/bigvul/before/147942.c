static void Uint8ArrayAttributeAttributeSetter(
    v8::Local<v8::Value> v8_value, const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  ALLOW_UNUSED_LOCAL(isolate);

  v8::Local<v8::Object> holder = info.Holder();
  ALLOW_UNUSED_LOCAL(holder);

  TestObject* impl = V8TestObject::ToImpl(holder);

  ExceptionState exception_state(isolate, ExceptionState::kSetterContext, "TestObject", "uint8ArrayAttribute");

  NotShared<DOMUint8Array> cpp_value = ToNotShared<NotShared<DOMUint8Array>>(info.GetIsolate(), v8_value, exception_state);
  if (exception_state.HadException())
    return;

  if (!cpp_value) {
    exception_state.ThrowTypeError("The provided value is not of type 'Uint8Array'.");
    return;
  }

  impl->setUint8ArrayAttribute(cpp_value);
}
