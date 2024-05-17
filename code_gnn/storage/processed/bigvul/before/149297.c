void DatabaseImpl::CreateTransaction(
    int64_t transaction_id,
    const std::vector<int64_t>& object_store_ids,
    blink::WebIDBTransactionMode mode) {
  idb_runner_->PostTask(
      FROM_HERE,
      base::Bind(&IDBThreadHelper::CreateTransaction, base::Unretained(helper_),
                 transaction_id, object_store_ids, mode));
}
