static void IndexedPropertySetter(
    uint32_t index,
    v8::Local<v8::Value> v8_value,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());
  V8StringResource<> property_value = v8_value;
  if (!property_value.Prepare())
    return;

  ScriptState* script_state = ScriptState::ForRelevantRealm(info);
  bool result = impl->setItem(script_state, index, property_value);
  if (!result)
    return;
  V8SetReturnValue(info, v8_value);
}
