void V8TestObject::CustomGetterReadonlyObjectAttributeAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_customGetterReadonlyObjectAttribute_Getter");

  V8TestObject::CustomGetterReadonlyObjectAttributeAttributeGetterCustom(info);
}
