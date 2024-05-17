void DatabaseImpl::Commit(int64_t transaction_id) {
  idb_runner_->PostTask(
      FROM_HERE, base::Bind(&IDBThreadHelper::Commit, base::Unretained(helper_),
                            transaction_id));
}
