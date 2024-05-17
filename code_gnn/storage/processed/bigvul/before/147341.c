void V8TestObject::DeprecatedLongAttributeAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_deprecatedLongAttribute_Getter");

  Deprecation::CountDeprecation(CurrentExecutionContext(info.GetIsolate()), WebFeature::kLongAttribute);

  test_object_v8_internal::DeprecatedLongAttributeAttributeGetter(info);
}
