void IndexedDBDispatcher::OnBlocked(int32 thread_id, int32 response_id) {
  DCHECK_EQ(thread_id, CurrentWorkerId());
  WebIDBCallbacks* callbacks = pending_callbacks_.Lookup(response_id);
  DCHECK(callbacks);
  callbacks->onBlocked();
}
