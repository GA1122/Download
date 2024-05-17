void V8TestObject::VoidMethodUint8ArrayArgMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_voidMethodUint8ArrayArg");

  test_object_v8_internal::VoidMethodUint8ArrayArgMethod(info);
}
