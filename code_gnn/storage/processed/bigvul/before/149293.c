void DatabaseImpl::CreateIndex(int64_t transaction_id,
                               int64_t object_store_id,
                               int64_t index_id,
                               const base::string16& name,
                               const IndexedDBKeyPath& key_path,
                               bool unique,
                               bool multi_entry) {
  idb_runner_->PostTask(
      FROM_HERE,
      base::Bind(&IDBThreadHelper::CreateIndex, base::Unretained(helper_),
                 transaction_id, object_store_id, index_id, name, key_path,
                 unique, multi_entry));
}
