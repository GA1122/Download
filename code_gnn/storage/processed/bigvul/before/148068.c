void V8TestObject::VoidMethodDefaultTrueBooleanArgMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_voidMethodDefaultTrueBooleanArg");

  test_object_v8_internal::VoidMethodDefaultTrueBooleanArgMethod(info);
}
