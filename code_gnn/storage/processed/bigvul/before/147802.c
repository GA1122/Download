void V8TestObject::RuntimeCallStatsCounterReadOnlyAttributeAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE(info.GetIsolate(), RuntimeCallStats::CounterId::kRuntimeCallStatsCounterReadOnlyAttribute_Getter);

  test_object_v8_internal::RuntimeCallStatsCounterReadOnlyAttributeAttributeGetter(info);
}
