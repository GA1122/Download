bool InitializeOnDBThread(int64* temporary_quota_override,
                          int64* desired_available_space,
                          QuotaDatabase* database) {
  DCHECK(database);
  database->GetQuotaConfigValue(QuotaDatabase::kTemporaryQuotaOverrideKey,
                                temporary_quota_override);
  database->GetQuotaConfigValue(QuotaDatabase::kDesiredAvailableSpaceKey,
                                desired_available_space);
  return true;
}
