void IndexedDBDispatcher::OnError(int32 thread_id, int32 response_id, int code,
                                  const string16& message) {
  DCHECK_EQ(thread_id, CurrentWorkerId());
  WebIDBCallbacks* callbacks = pending_callbacks_.Lookup(response_id);
  if (!callbacks)
    return;
  callbacks->onError(WebIDBDatabaseError(code, message));
  pending_callbacks_.Remove(response_id);
}
