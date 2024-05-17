void IndexedDBDispatcher::OnSuccessSerializedScriptValue(
    int32 thread_id, int32 response_id,
    const content::SerializedScriptValue& value) {
  DCHECK_EQ(thread_id, CurrentWorkerId());
  WebIDBCallbacks* callbacks = pending_callbacks_.Lookup(response_id);
  if (!callbacks)
    return;
  callbacks->onSuccess(value);
  pending_callbacks_.Remove(response_id);
}
