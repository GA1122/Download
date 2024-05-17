void V8TestObject::CallWithScriptStateLongMethodMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_callWithScriptStateLongMethod");

  test_object_v8_internal::CallWithScriptStateLongMethodMethod(info);
}
