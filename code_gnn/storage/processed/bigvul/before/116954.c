void IndexedDBDispatcher::RequestIDBCursorPrefetchReset(
    int used_prefetches, int unused_prefetches, int32 idb_cursor_id) {
  Send(new IndexedDBHostMsg_CursorPrefetchReset(idb_cursor_id,
                                                used_prefetches,
                                                unused_prefetches));
}
