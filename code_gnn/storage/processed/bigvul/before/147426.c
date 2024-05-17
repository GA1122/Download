void V8TestObject::HighEntropyMethodWithMeasureMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_highEntropyMethodWithMeasure");

  ExecutionContext* execution_context_for_measurement = CurrentExecutionContext(info.GetIsolate());
  UseCounter::Count(execution_context_for_measurement, WebFeature::kV8TestObject_HighEntropyMethodWithMeasure_Method);
  Dactyloscoper::Record(execution_context_for_measurement, WebFeature::kV8TestObject_HighEntropyMethodWithMeasure_Method);
  test_object_v8_internal::HighEntropyMethodWithMeasureMethod(info);
}
