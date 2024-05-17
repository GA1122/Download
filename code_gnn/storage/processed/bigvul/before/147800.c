void V8TestObject::RuntimeCallStatsCounterMethodMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE(info.GetIsolate(), RuntimeCallStats::CounterId::kRuntimeCallStatsCounterMethod);
  test_object_v8_internal::RuntimeCallStatsCounterMethodMethod(info);
}
