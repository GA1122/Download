void DatabaseImpl::VersionChangeIgnored() {
  idb_runner_->PostTask(FROM_HERE,
                        base::Bind(&IDBThreadHelper::VersionChangeIgnored,
                                   base::Unretained(helper_)));
}
