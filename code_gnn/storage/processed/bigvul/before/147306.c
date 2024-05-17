void V8TestObject::CustomObjectAttributeAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_customObjectAttribute_Getter");

  V8TestObject::CustomObjectAttributeAttributeGetterCustom(info);
}
