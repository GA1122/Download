void IndexedDBDatabase::CreateObjectStoreAbortOperation(
    int64_t object_store_id) {
  IDB_TRACE("IndexedDBDatabase::CreateObjectStoreAbortOperation");
  RemoveObjectStore(object_store_id);
}
