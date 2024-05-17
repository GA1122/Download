void V8TestObject::Float32ArrayMethodMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_float32ArrayMethod");

  test_object_v8_internal::Float32ArrayMethodMethod(info);
}
