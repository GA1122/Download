static void CallWithScriptStateLongMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  ScriptState* script_state = ScriptState::ForRelevantRealm(info);

  int32_t result = impl->callWithScriptStateLongMethod(script_state);
  V8SetReturnValueInt(info, result);
}
