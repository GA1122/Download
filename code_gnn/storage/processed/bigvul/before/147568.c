static void NamedPropertySetter(
    const AtomicString& name,
    v8::Local<v8::Value> v8_value,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
  ScriptState* script_state = ScriptState::ForRelevantRealm(info);

  TestObject* impl = V8TestObject::ToImpl(info.Holder());
  V8StringResource<> property_value = v8_value;
  if (!property_value.Prepare())
    return;

  bool result = impl->AnonymousNamedSetter(script_state, name, property_value);
  if (!result)
    return;
  V8SetReturnValue(info, v8_value);
}
