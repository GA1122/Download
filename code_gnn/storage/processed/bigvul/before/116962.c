void IndexedDBDispatcher::RequestIDBIndexCount(
    const WebIDBKeyRange& idb_key_range,
    WebIDBCallbacks* callbacks_ptr,
    int32 idb_index_id,
    const WebIDBTransaction& transaction,
    WebExceptionCode* ec) {
  ResetCursorPrefetchCaches();
  scoped_ptr<WebIDBCallbacks> callbacks(callbacks_ptr);
  IndexedDBHostMsg_IndexCount_Params params;
  params.thread_id = CurrentWorkerId();
  params.response_id = pending_callbacks_.Add(callbacks.release());
  params.lower_key.Set(idb_key_range.lower());
  params.upper_key.Set(idb_key_range.upper());
  params.lower_open = idb_key_range.lowerOpen();
  params.upper_open = idb_key_range.upperOpen();
  params.idb_index_id = idb_index_id;
  params.transaction_id = TransactionId(transaction);
  Send(new IndexedDBHostMsg_IndexCount(params, ec));
  if (*ec)
    pending_callbacks_.Remove(params.response_id);
}
