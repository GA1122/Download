void IndexedDBDispatcher::OnSuccessIDBDatabase(int32 thread_id,
                                               int32 response_id,
                                               int32 object_id) {
  DCHECK_EQ(thread_id, CurrentWorkerId());
  WebIDBCallbacks* callbacks = pending_callbacks_.Lookup(response_id);
  if (!callbacks)
    return;
  callbacks->onSuccess(new RendererWebIDBDatabaseImpl(object_id));
  pending_callbacks_.Remove(response_id);
}
