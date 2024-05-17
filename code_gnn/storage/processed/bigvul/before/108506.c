void QuotaManager::GetPersistentHostQuota(const std::string& host,
                                          const HostQuotaCallback& callback) {
  LazyInitialize();
  if (host.empty()) {
    callback.Run(kQuotaStatusOk, host, kStorageTypePersistent, 0);
    return;
  }

  int64* quota_ptr = new int64(0);
  PostTaskAndReplyWithResultForDBThread(
      FROM_HERE,
      base::Bind(&GetPersistentHostQuotaOnDBThread,
                 host,
                 base::Unretained(quota_ptr)),
      base::Bind(&QuotaManager::DidGetPersistentHostQuota,
                 weak_factory_.GetWeakPtr(),
                 callback,
                 host,
                 base::Owned(quota_ptr)));
}
