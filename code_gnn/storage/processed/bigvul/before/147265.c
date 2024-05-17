static void CallWithScriptStateAnyAttributeAttributeSetter(
    v8::Local<v8::Value> v8_value, const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  ALLOW_UNUSED_LOCAL(isolate);

  v8::Local<v8::Object> holder = info.Holder();
  ALLOW_UNUSED_LOCAL(holder);

  TestObject* impl = V8TestObject::ToImpl(holder);

  ScriptValue cpp_value = ScriptValue(info.GetIsolate(), v8_value);

  ScriptState* script_state = ScriptState::ForRelevantRealm(info);

  impl->setCallWithScriptStateAnyAttribute(script_state, cpp_value);
}
