void QuotaManager::DeleteHostData(const std::string& host,
                                  StorageType type,
                                  int quota_client_mask,
                                  const StatusCallback& callback) {
  LazyInitialize();

  if (host.empty() || clients_.empty()) {
    callback.Run(kQuotaStatusOk);
    return;
  }

  HostDataDeleter* deleter =
      new HostDataDeleter(this, host, type, quota_client_mask, callback);
  deleter->Start();
}
