void IndexedDBDispatcher::RequestIDBObjectStoreDeleteRange(
    const IndexedDBKeyRange& key_range,
    WebIDBCallbacks* callbacks_ptr,
    int32 idb_object_store_id,
    const WebIDBTransaction& transaction,
    WebExceptionCode* ec) {
  ResetCursorPrefetchCaches();
  scoped_ptr<WebIDBCallbacks> callbacks(callbacks_ptr);

  int32 response_id = pending_callbacks_.Add(callbacks.release());
  Send(new IndexedDBHostMsg_ObjectStoreDeleteRange(
      idb_object_store_id, CurrentWorkerId(), response_id, key_range,
      TransactionId(transaction), ec));
  if (*ec)
    pending_callbacks_.Remove(response_id);
}
