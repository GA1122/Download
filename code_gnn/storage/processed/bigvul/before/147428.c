void V8TestObject::HighEntropyReadonlyAttributeWithMeasureAsAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_highEntropyReadonlyAttributeWithMeasureAs_Getter");

  ExecutionContext* execution_context_for_measurement = CurrentExecutionContext(info.GetIsolate());
  UseCounter::Count(execution_context_for_measurement, WebFeature::kTestAttributeHighEntropy);
  Dactyloscoper::Record(execution_context_for_measurement, WebFeature::kTestAttributeHighEntropy);

  test_object_v8_internal::HighEntropyReadonlyAttributeWithMeasureAsAttributeGetter(info);
}
