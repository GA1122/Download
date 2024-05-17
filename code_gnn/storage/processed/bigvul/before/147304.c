void V8TestObject::CustomImplementedAsLongAttributeAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_customImplementedAsLongAttribute_Getter");

  V8TestObject::CustomImplementedAsLongAttributeAttributeGetterCustom(info);
}
