void V8TestObject::IndexedPropertyGetterCallback(
    uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_IndexedPropertyGetter");

  test_object_v8_internal::IndexedPropertyGetter(index, info);
}
