void V8TestObject::NewObjectTestInterfacePromiseMethodMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_newObjectTestInterfacePromiseMethod");

  test_object_v8_internal::NewObjectTestInterfacePromiseMethodMethod(info);
}
