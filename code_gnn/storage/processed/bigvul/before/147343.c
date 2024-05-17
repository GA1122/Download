void V8TestObject::DeprecatedLongAttributeAttributeSetterCallback(
    const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_deprecatedLongAttribute_Setter");

  v8::Local<v8::Value> v8_value = info[0];

  Deprecation::CountDeprecation(CurrentExecutionContext(info.GetIsolate()), WebFeature::kLongAttribute);

  test_object_v8_internal::DeprecatedLongAttributeAttributeSetter(v8_value, info);
}
