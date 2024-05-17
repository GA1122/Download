void IndexedDBDispatcher::RequestIDBObjectStoreOpenCursor(
    const WebIDBKeyRange& idb_key_range,
    unsigned short direction,
    WebIDBCallbacks* callbacks_ptr,
    int32 idb_object_store_id,
    const WebIDBTransaction& transaction,
    WebExceptionCode* ec) {
  ResetCursorPrefetchCaches();
  scoped_ptr<WebIDBCallbacks> callbacks(callbacks_ptr);
  IndexedDBHostMsg_ObjectStoreOpenCursor_Params params;
  params.thread_id = CurrentWorkerId();
  params.response_id = pending_callbacks_.Add(callbacks.release());
  params.lower_key.Set(idb_key_range.lower());
  params.upper_key.Set(idb_key_range.upper());
  params.lower_open = idb_key_range.lowerOpen();
  params.upper_open = idb_key_range.upperOpen();
  params.direction = direction;
  params.idb_object_store_id = idb_object_store_id;
  params.transaction_id = TransactionId(transaction);
  Send(new IndexedDBHostMsg_ObjectStoreOpenCursor(params, ec));
  if (*ec)
    pending_callbacks_.Remove(params.response_id);
}
