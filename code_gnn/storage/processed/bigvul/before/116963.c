void IndexedDBDispatcher::RequestIDBIndexGetKey(
    const IndexedDBKey& key,
    WebIDBCallbacks* callbacks_ptr,
    int32 idb_index_id,
    const WebIDBTransaction& transaction,
    WebExceptionCode* ec) {
  ResetCursorPrefetchCaches();
  scoped_ptr<WebIDBCallbacks> callbacks(callbacks_ptr);
  int32 response_id = pending_callbacks_.Add(callbacks.release());
  Send(new IndexedDBHostMsg_IndexGetKey(
           idb_index_id, CurrentWorkerId(), response_id, key,
           TransactionId(transaction), ec));
  if (*ec)
    pending_callbacks_.Remove(response_id);
}
