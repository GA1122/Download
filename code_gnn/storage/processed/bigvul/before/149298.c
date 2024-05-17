void DatabaseImpl::DeleteIndex(int64_t transaction_id,
                               int64_t object_store_id,
                               int64_t index_id) {
  idb_runner_->PostTask(
      FROM_HERE,
      base::Bind(&IDBThreadHelper::DeleteIndex, base::Unretained(helper_),
                 transaction_id, object_store_id, index_id));
}
