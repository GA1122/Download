bool IndexedDBDatabase::ValidateObjectStoreIdAndNewIndexId(
    int64_t object_store_id,
    int64_t index_id) const {
  if (!ValidateObjectStoreId(object_store_id))
    return false;
  const IndexedDBObjectStoreMetadata& object_store_metadata =
      metadata_.object_stores.find(object_store_id)->second;
  if (base::ContainsKey(object_store_metadata.indexes, index_id)) {
    DLOG(ERROR) << "Invalid index_id";
    return false;
  }
  return true;
}
