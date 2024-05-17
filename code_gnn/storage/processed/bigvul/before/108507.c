bool GetPersistentHostQuotaOnDBThread(const std::string& host,
                                      int64* quota,
                                      QuotaDatabase* database) {
  DCHECK(database);
  database->GetHostQuota(host, kStorageTypePersistent, quota);
  return true;
}
