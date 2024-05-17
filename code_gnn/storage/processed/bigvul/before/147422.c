void V8TestObject::HighEntropyConstantConstantGetterCallback(v8::Local<v8::Name>, const v8::PropertyCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_highEntropyConstant_ConstantGetter");

  ExecutionContext* execution_context_for_measurement = CurrentExecutionContext(info.GetIsolate());
  UseCounter::Count(execution_context_for_measurement, WebFeature::kTestFeatureHighEntropyConstant);
  Dactyloscoper::Record(execution_context_for_measurement, WebFeature::kTestFeatureHighEntropyConstant);
  V8SetReturnValueString(info, "1");
}
