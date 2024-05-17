void DatabaseImpl::Abort(int64_t transaction_id) {
  idb_runner_->PostTask(
      FROM_HERE, base::Bind(&IDBThreadHelper::Abort, base::Unretained(helper_),
                            transaction_id));
}
