void V8TestObject::HTMLCollectionAttributeAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_htmlCollectionAttribute_Getter");

  test_object_v8_internal::HTMLCollectionAttributeAttributeGetter(info);
}
