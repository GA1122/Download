static void Float32ArrayAttributeAttributeSetter(
    v8::Local<v8::Value> v8_value, const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  ALLOW_UNUSED_LOCAL(isolate);

  v8::Local<v8::Object> holder = info.Holder();
  ALLOW_UNUSED_LOCAL(holder);

  TestObject* impl = V8TestObject::ToImpl(holder);

  ExceptionState exception_state(isolate, ExceptionState::kSetterContext, "TestObject", "float32ArrayAttribute");

  NotShared<DOMFloat32Array> cpp_value = ToNotShared<NotShared<DOMFloat32Array>>(info.GetIsolate(), v8_value, exception_state);
  if (exception_state.HadException())
    return;

  if (!cpp_value) {
    exception_state.ThrowTypeError("The provided value is not of type 'Float32Array'.");
    return;
  }

  impl->setFloat32ArrayAttribute(cpp_value);
}
