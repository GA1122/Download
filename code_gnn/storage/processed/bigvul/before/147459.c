void V8TestObject::Int32ArrayMethodMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_int32ArrayMethod");

  test_object_v8_internal::Int32ArrayMethodMethod(info);
}
