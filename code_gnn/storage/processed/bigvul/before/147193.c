void V8TestObject::ActivityLoggingSetterForAllWorldsLongAttributeAttributeSetterCallback(
    const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TestObject_activityLoggingSetterForAllWorldsLongAttribute_Setter");

  v8::Local<v8::Value> v8_value = info[0];

  ScriptState* script_state = ScriptState::ForRelevantRealm(info);
  V8PerContextData* context_data = script_state->PerContextData();
  if (context_data && context_data->ActivityLogger()) {
    context_data->ActivityLogger()->LogSetter("TestObject.activityLoggingSetterForAllWorldsLongAttribute", v8_value);
  }

  test_object_v8_internal::ActivityLoggingSetterForAllWorldsLongAttributeAttributeSetter(v8_value, info);
}