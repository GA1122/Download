void V8TestObject::CachedArrayAttributeAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_cachedArrayAttribute_Getter");

  test_object_v8_internal::CachedArrayAttributeAttributeGetter(info);
}
