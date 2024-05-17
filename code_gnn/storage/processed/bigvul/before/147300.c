void V8TestObject::CustomGetterLongAttributeAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_customGetterLongAttribute_Getter");

  V8TestObject::CustomGetterLongAttributeAttributeGetterCustom(info);
}
