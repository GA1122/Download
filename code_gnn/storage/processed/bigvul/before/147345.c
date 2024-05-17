void V8TestObject::DeprecatedVoidMethodMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_deprecatedVoidMethod");

  Deprecation::CountDeprecation(CurrentExecutionContext(info.GetIsolate()), WebFeature::kvoidMethod);
  test_object_v8_internal::DeprecatedVoidMethodMethod(info);
}
