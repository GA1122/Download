void V8TestObject::HighEntropyReadonlyAttributeWithMeasureAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_highEntropyReadonlyAttributeWithMeasure_Getter");

  ExecutionContext* execution_context_for_measurement = CurrentExecutionContext(info.GetIsolate());
  UseCounter::Count(execution_context_for_measurement, WebFeature::kV8TestObject_HighEntropyReadonlyAttributeWithMeasure_AttributeGetter);
  Dactyloscoper::Record(execution_context_for_measurement, WebFeature::kV8TestObject_HighEntropyReadonlyAttributeWithMeasure_AttributeGetter);

  test_object_v8_internal::HighEntropyReadonlyAttributeWithMeasureAttributeGetter(info);
}
