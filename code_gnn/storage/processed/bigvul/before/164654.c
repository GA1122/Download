void IndexedDBDatabase::RenameObjectStoreAbortOperation(
    int64_t object_store_id,
    base::string16 old_name) {
  IDB_TRACE("IndexedDBDatabase::RenameObjectStoreAbortOperation");

  DCHECK(metadata_.object_stores.find(object_store_id) !=
         metadata_.object_stores.end());
  metadata_.object_stores[object_store_id].name = std::move(old_name);
}
