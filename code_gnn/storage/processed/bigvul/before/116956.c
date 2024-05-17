void IndexedDBDispatcher::RequestIDBDatabaseClose(int32 idb_database_id) {
  ResetCursorPrefetchCaches();
  Send(new IndexedDBHostMsg_DatabaseClose(idb_database_id));
  pending_database_callbacks_.Remove(idb_database_id);
}
