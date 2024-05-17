void V8TestObject::HighEntropyAttributeWithMeasureAttributeSetterCallback(
    const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_highEntropyAttributeWithMeasure_Setter");

  v8::Local<v8::Value> v8_value = info[0];

  UseCounter::Count(CurrentExecutionContext(info.GetIsolate()), WebFeature::kV8TestObject_HighEntropyAttributeWithMeasure_AttributeSetter);

  test_object_v8_internal::HighEntropyAttributeWithMeasureAttributeSetter(v8_value, info);
}