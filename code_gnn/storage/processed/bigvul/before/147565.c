static void NamedPropertyGetter(const AtomicString& name,
                                const v8::PropertyCallbackInfo<v8::Value>& info) {
  ScriptState* script_state = ScriptState::ForRelevantRealm(info);

  TestObject* impl = V8TestObject::ToImpl(info.Holder());
  ScriptValue result = impl->AnonymousNamedGetter(script_state, name);
  if (result.IsEmpty())
    return;
  V8SetReturnValue(info, result.V8Value());
}
