void QuotaManager::GetUsageInfo(const GetUsageInfoCallback& callback) {
  LazyInitialize();
  GetUsageInfoTask* get_usage_info = new GetUsageInfoTask(this, callback);
  get_usage_info->Start();
}
