void V8TestObject::PromiseOverloadMethodMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_promiseOverloadMethod");

  test_object_v8_internal::PromiseOverloadMethodMethod(info);
}
