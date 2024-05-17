void V8TestObject::CustomSetterImplementedAsLongAttributeAttributeSetterCallback(
    const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_customSetterImplementedAsLongAttribute_Setter");

  v8::Local<v8::Value> v8_value = info[0];

  V8TestObject::CustomSetterImplementedAsLongAttributeAttributeSetterCustom(v8_value, info);
}
