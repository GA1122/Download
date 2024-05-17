void StoragePartitionImpl::GetQuotaSettings(
    storage::OptionalQuotaSettingsCallback callback) {
  GetContentClient()->browser()->GetQuotaSettings(browser_context_, this,
                                                  std::move(callback));
}
