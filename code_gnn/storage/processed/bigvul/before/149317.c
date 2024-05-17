void DatabaseImpl::RenameObjectStore(int64_t transaction_id,
                                     int64_t object_store_id,
                                     const base::string16& new_name) {
  idb_runner_->PostTask(
      FROM_HERE,
      base::Bind(&IDBThreadHelper::RenameObjectStore, base::Unretained(helper_),
                 transaction_id, object_store_id, new_name));
}
