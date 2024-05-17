void IndexedDBDatabase::CreateIndexAbortOperation(int64_t object_store_id,
                                                  int64_t index_id) {
  IDB_TRACE("IndexedDBDatabase::CreateIndexAbortOperation");
  RemoveIndex(object_store_id, index_id);
}
