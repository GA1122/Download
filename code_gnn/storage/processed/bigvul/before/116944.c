void IndexedDBDispatcher::OnSuccessIndexedDBKey(int32 thread_id,
                                                int32 response_id,
                                                const IndexedDBKey& key) {
  DCHECK_EQ(thread_id, CurrentWorkerId());
  WebIDBCallbacks* callbacks = pending_callbacks_.Lookup(response_id);
  if (!callbacks)
    return;
  callbacks->onSuccess(key);
  pending_callbacks_.Remove(response_id);
}
