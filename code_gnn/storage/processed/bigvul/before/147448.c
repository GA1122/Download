void V8TestObject::IndexedPropertyDescriptorCallback(
    uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info) {
  test_object_v8_internal::IndexedPropertyDescriptor(index, info);
}
