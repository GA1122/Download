void V8TestObject::XmlAttributeAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_xmlAttribute_Getter");

  test_object_v8_internal::XmlAttributeAttributeGetter(info);
}
