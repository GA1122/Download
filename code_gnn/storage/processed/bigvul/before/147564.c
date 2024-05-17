void V8TestObject::NamedPropertyEnumeratorCallback(
    const v8::PropertyCallbackInfo<v8::Array>& info) {
  test_object_v8_internal::NamedPropertyEnumerator(info);
}
