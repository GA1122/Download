void V8TestObject::CachedStringOrNoneAttributeAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_cachedStringOrNoneAttribute_Getter");

  test_object_v8_internal::CachedStringOrNoneAttributeAttributeGetter(info);
}
