void V8TestObject::MeasureAsFeatureNameTestInterfaceEmptyConstructorAttributeConstructorGetterCallback(
    v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_measureAsFeatureNameTestInterfaceEmptyConstructorAttribute_ConstructorGetterCallback");

  ExecutionContext* execution_context_for_measurement = CurrentExecutionContext(info.GetIsolate());
  UseCounter::Count(execution_context_for_measurement, WebFeature::kFeatureName);

  V8ConstructorAttributeGetter(property, info, V8TestInterfaceEmpty::GetWrapperTypeInfo());
}
