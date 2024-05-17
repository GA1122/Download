void InvokeLocalStorageUsageCallbackHelper(
    DOMStorageContext::GetLocalStorageUsageCallback callback,
    std::unique_ptr<std::vector<StorageUsageInfo>> infos) {
  std::move(callback).Run(*infos);
}
