static void DoubleOrNullStringAttributeAttributeSetter(
    v8::Local<v8::Value> v8_value, const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  ALLOW_UNUSED_LOCAL(isolate);

  v8::Local<v8::Object> holder = info.Holder();
  ALLOW_UNUSED_LOCAL(holder);

  TestObject* impl = V8TestObject::ToImpl(holder);

  ExceptionState exception_state(isolate, ExceptionState::kSetterContext, "TestObject", "doubleOrNullStringAttribute");

  DoubleOrString cpp_value;
  V8DoubleOrString::ToImpl(info.GetIsolate(), v8_value, cpp_value, UnionTypeConversionMode::kNullable, exception_state);
  if (exception_state.HadException())
    return;

  impl->setDoubleOrNullStringAttribute(cpp_value);
}
