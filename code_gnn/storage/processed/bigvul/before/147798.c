void V8TestObject::RuntimeCallStatsCounterAttributeAttributeSetterCallback(
    const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE(info.GetIsolate(), RuntimeCallStats::CounterId::kRuntimeCallStatsCounterAttribute_Setter);

  v8::Local<v8::Value> v8_value = info[0];

  test_object_v8_internal::RuntimeCallStatsCounterAttributeAttributeSetter(v8_value, info);
}
