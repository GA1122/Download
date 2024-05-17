static void IndexedPropertyGetter(
    uint32_t index,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (index >= impl->length())
    return;   

  ScriptState* script_state = ScriptState::ForRelevantRealm(info);
  ScriptValue result = impl->item(script_state, index);
  V8SetReturnValue(info, result.V8Value());
}
