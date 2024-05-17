void IndexedDBDatabase::DeleteObjectStoreAbortOperation(
    IndexedDBObjectStoreMetadata object_store_metadata) {
  IDB_TRACE("IndexedDBDatabase::DeleteObjectStoreAbortOperation");
  AddObjectStore(std::move(object_store_metadata),
                 IndexedDBObjectStoreMetadata::kInvalidId);
}
