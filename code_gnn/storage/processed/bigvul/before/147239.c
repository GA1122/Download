static void CachedArrayAttributeAttributeSetter(
    v8::Local<v8::Value> v8_value, const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  ALLOW_UNUSED_LOCAL(isolate);

  v8::Local<v8::Object> holder = info.Holder();
  ALLOW_UNUSED_LOCAL(holder);

  TestObject* impl = V8TestObject::ToImpl(holder);

  ExceptionState exception_state(isolate, ExceptionState::kSetterContext, "TestObject", "cachedArrayAttribute");

  Vector<String> cpp_value = NativeValueTraits<IDLSequence<IDLString>>::NativeValue(info.GetIsolate(), v8_value, exception_state);
  if (exception_state.HadException())
    return;

  impl->setCachedArrayAttribute(cpp_value);

  V8PrivateProperty::GetSymbol(
      isolate,
      kPrivatePropertyCachedArrayAttribute)
      .DeleteProperty(holder);
}
