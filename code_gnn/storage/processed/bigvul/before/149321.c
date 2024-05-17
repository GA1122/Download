void DatabaseImpl::SetIndexesReady(int64_t transaction_id,
                                   int64_t object_store_id,
                                   const std::vector<int64_t>& index_ids) {
  idb_runner_->PostTask(
      FROM_HERE,
      base::Bind(&IDBThreadHelper::SetIndexesReady, base::Unretained(helper_),
                 transaction_id, object_store_id, index_ids));
}
