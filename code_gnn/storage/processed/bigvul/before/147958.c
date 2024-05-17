static void UnrestrictedDoubleOrStringAttributeAttributeSetter(
    v8::Local<v8::Value> v8_value, const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  ALLOW_UNUSED_LOCAL(isolate);

  v8::Local<v8::Object> holder = info.Holder();
  ALLOW_UNUSED_LOCAL(holder);

  TestObject* impl = V8TestObject::ToImpl(holder);

  ExceptionState exception_state(isolate, ExceptionState::kSetterContext, "TestObject", "unrestrictedDoubleOrStringAttribute");

  UnrestrictedDoubleOrString cpp_value;
  V8UnrestrictedDoubleOrString::ToImpl(info.GetIsolate(), v8_value, cpp_value, UnionTypeConversionMode::kNotNullable, exception_state);
  if (exception_state.HadException())
    return;

  impl->setUnrestrictedDoubleOrStringAttribute(cpp_value);
}