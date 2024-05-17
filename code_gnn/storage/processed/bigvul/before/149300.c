void DatabaseImpl::DeleteObjectStore(int64_t transaction_id,
                                     int64_t object_store_id) {
  idb_runner_->PostTask(
      FROM_HERE,
      base::Bind(&IDBThreadHelper::DeleteObjectStore, base::Unretained(helper_),
                 transaction_id, object_store_id));
}
