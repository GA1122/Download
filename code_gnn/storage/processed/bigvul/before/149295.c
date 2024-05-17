void DatabaseImpl::CreateObjectStore(int64_t transaction_id,
                                     int64_t object_store_id,
                                     const base::string16& name,
                                     const IndexedDBKeyPath& key_path,
                                     bool auto_increment) {
  idb_runner_->PostTask(
      FROM_HERE, base::Bind(&IDBThreadHelper::CreateObjectStore,
                            base::Unretained(helper_), transaction_id,
                            object_store_id, name, key_path, auto_increment));
}
