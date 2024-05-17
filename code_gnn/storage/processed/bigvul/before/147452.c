void V8TestObject::IndexedPropertySetterCallback(
    uint32_t index,
    v8::Local<v8::Value> v8_value,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
  test_object_v8_internal::IndexedPropertySetter(index, v8_value, info);
}
