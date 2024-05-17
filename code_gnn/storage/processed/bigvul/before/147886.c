void V8TestObject::StringSequenceMethodMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_stringSequenceMethod");

  test_object_v8_internal::StringSequenceMethodMethod(info);
}
