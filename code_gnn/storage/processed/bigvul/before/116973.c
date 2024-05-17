void IndexedDBDispatcher::RequestIDBObjectStorePut(
    const content::SerializedScriptValue& value,
    const IndexedDBKey& key,
    WebKit::WebIDBObjectStore::PutMode put_mode,
    WebIDBCallbacks* callbacks_ptr,
    int32 idb_object_store_id,
    const WebIDBTransaction& transaction,
    WebExceptionCode* ec) {
  ResetCursorPrefetchCaches();
  scoped_ptr<WebIDBCallbacks> callbacks(callbacks_ptr);
  if (!value.is_null() &&
      (value.data().length() * sizeof(char16)) > kMaxIDBValueSizeInBytes) {
    *ec = WebKit::WebIDBDatabaseExceptionDataError;
    return;
  }
  IndexedDBHostMsg_ObjectStorePut_Params params;
  params.thread_id = CurrentWorkerId();
  params.idb_object_store_id = idb_object_store_id;
  params.response_id = pending_callbacks_.Add(callbacks.release());
  params.serialized_value = value;
  params.key = key;
  params.put_mode = put_mode;
  params.transaction_id = TransactionId(transaction);
  Send(new IndexedDBHostMsg_ObjectStorePut(params, ec));
  if (*ec)
    pending_callbacks_.Remove(params.response_id);
}
