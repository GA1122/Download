void DatabaseImpl::Close() {
  idb_runner_->PostTask(FROM_HERE, base::Bind(&IDBThreadHelper::Close,
                                              base::Unretained(helper_)));
}
