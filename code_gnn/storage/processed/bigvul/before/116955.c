void IndexedDBDispatcher::RequestIDBCursorUpdate(
    const content::SerializedScriptValue& value,
    WebIDBCallbacks* callbacks_ptr,
    int32 idb_cursor_id,
    WebExceptionCode* ec) {
  ResetCursorPrefetchCaches();
  scoped_ptr<WebIDBCallbacks> callbacks(callbacks_ptr);
  if (!value.is_null() &&
      (value.data().length() * sizeof(char16)) > kMaxIDBValueSizeInBytes) {
    *ec = WebKit::WebIDBDatabaseExceptionDataError;
    return;
  }
  int32 response_id = pending_callbacks_.Add(callbacks.release());
  Send(
      new IndexedDBHostMsg_CursorUpdate(idb_cursor_id, CurrentWorkerId(),
                                        response_id, value, ec));
  if (*ec)
    pending_callbacks_.Remove(response_id);
}
