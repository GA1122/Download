void V8TestObject::IndexedPropertyDeleterCallback(
    uint32_t index, const v8::PropertyCallbackInfo<v8::Boolean>& info) {
  test_object_v8_internal::IndexedPropertyDeleter(index, info);
}
