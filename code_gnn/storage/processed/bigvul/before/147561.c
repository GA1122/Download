static void NamedPropertyDeleter(
    const AtomicString& name, const v8::PropertyCallbackInfo<v8::Boolean>& info) {
  ScriptState* script_state = ScriptState::ForRelevantRealm(info);

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  DeleteResult result = impl->AnonymousNamedDeleter(script_state, name);
  if (result == kDeleteUnknownProperty)
    return;
  V8SetReturnValue(info, result == kDeleteSuccess);
}
