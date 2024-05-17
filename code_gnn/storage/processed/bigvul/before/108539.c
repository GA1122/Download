bool SetPersistentHostQuotaOnDBThread(const std::string& host,
                                      int64* new_quota,
                                      QuotaDatabase* database) {
  DCHECK(database);
  if (database->SetHostQuota(host, kStorageTypePersistent, *new_quota))
    return true;
  *new_quota = 0;
  return false;
}
