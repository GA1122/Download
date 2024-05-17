void IndexedDBDatabase::RenameIndexAbortOperation(int64_t object_store_id,
                                                  int64_t index_id,
                                                  base::string16 old_name) {
  IDB_TRACE("IndexedDBDatabase::RenameIndexAbortOperation");

  DCHECK(metadata_.object_stores.find(object_store_id) !=
         metadata_.object_stores.end());
  IndexedDBObjectStoreMetadata& object_store =
      metadata_.object_stores[object_store_id];

  DCHECK(object_store.indexes.find(index_id) != object_store.indexes.end());
  object_store.indexes[index_id].name = std::move(old_name);
}
