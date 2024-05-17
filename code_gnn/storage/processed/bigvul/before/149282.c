void DatabaseImpl::AddObserver(int64_t transaction_id,
                               int32_t observer_id,
                               bool include_transaction,
                               bool no_records,
                               bool values,
                               uint16_t operation_types) {
  idb_runner_->PostTask(
      FROM_HERE,
      base::Bind(&IDBThreadHelper::AddObserver, base::Unretained(helper_),
                 transaction_id, observer_id, include_transaction, no_records,
                 values, operation_types));
}
