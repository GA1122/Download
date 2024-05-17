void IndexedDBDatabase::DeleteIndexAbortOperation(
    int64_t object_store_id,
    IndexedDBIndexMetadata index_metadata) {
  IDB_TRACE("IndexedDBDatabase::DeleteIndexAbortOperation");
  AddIndex(object_store_id, std::move(index_metadata),
           IndexedDBIndexMetadata::kInvalidId);
}
