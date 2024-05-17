void V8TestObject::OctetAttributeAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_octetAttribute_Getter");

  test_object_v8_internal::OctetAttributeAttributeGetter(info);
}
