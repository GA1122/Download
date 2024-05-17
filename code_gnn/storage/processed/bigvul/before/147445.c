static void IndexedPropertyDeleter(
    uint32_t index, const v8::PropertyCallbackInfo<v8::Boolean>& info) {
  ExceptionState exception_state(
      info.GetIsolate(),
      ExceptionState::kIndexedDeletionContext,
      "TestObject");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  ScriptState* script_state = ScriptState::ForRelevantRealm(info);
  DeleteResult result = impl->AnonymousIndexedDeleter(script_state, index, exception_state);
  if (exception_state.HadException())
    return;
  if (result == kDeleteUnknownProperty)
    return;
  V8SetReturnValue(info, result == kDeleteSuccess);
}
