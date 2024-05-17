void V8TestObject::VoidMethodTestInterfaceEmptyFrozenArrayMethodMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_voidMethodTestInterfaceEmptyFrozenArrayMethod");

  test_object_v8_internal::VoidMethodTestInterfaceEmptyFrozenArrayMethodMethod(info);
}
