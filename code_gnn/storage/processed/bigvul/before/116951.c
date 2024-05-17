void IndexedDBDispatcher::RequestIDBCursorContinue(
    const IndexedDBKey& key,
    WebIDBCallbacks* callbacks_ptr,
    int32 idb_cursor_id,
    WebExceptionCode* ec) {
  ResetCursorPrefetchCaches(idb_cursor_id);

  scoped_ptr<WebIDBCallbacks> callbacks(callbacks_ptr);

  int32 response_id = pending_callbacks_.Add(callbacks.release());
  Send(
      new IndexedDBHostMsg_CursorContinue(idb_cursor_id, CurrentWorkerId(),
                                          response_id, key, ec));
  if (*ec)
    pending_callbacks_.Remove(response_id);
}
