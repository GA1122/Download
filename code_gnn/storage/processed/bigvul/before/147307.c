void V8TestObject::CustomObjectAttributeAttributeSetterCallback(
    const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_customObjectAttribute_Setter");

  v8::Local<v8::Value> v8_value = info[0];

  V8TestObject::CustomObjectAttributeAttributeSetterCustom(v8_value, info);
}
