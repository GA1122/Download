void IndexedDBDispatcher::OnVersionChange(int32 thread_id,
                                          int32 database_id,
                                          const string16& newVersion) {
  DCHECK_EQ(thread_id, CurrentWorkerId());
  WebIDBDatabaseCallbacks* callbacks =
      pending_database_callbacks_.Lookup(database_id);
  if (!callbacks)
    return;
  callbacks->onVersionChange(newVersion);
}
