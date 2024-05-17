void QuotaManager::DidInitialize(int64* temporary_quota_override,
                                 int64* desired_available_space,
                                 bool success) {
  temporary_quota_override_ = *temporary_quota_override;
  desired_available_space_ = *desired_available_space;
  temporary_quota_initialized_ = true;
  DidDatabaseWork(success);

  histogram_timer_.Start(FROM_HERE,
                         base::TimeDelta::FromMilliseconds(
                             kReportHistogramInterval),
                         this, &QuotaManager::ReportHistogram);

  DCHECK(temporary_quota_initialized_);

  for (UsageAndQuotaDispatcherTaskMap::iterator iter =
           usage_and_quota_dispatchers_.begin();
       iter != usage_and_quota_dispatchers_.end(); ++iter) {
    if (iter->second->IsStartable())
      iter->second->Start();
  }

  GetTemporaryGlobalQuota(
      base::Bind(&QuotaManager::DidGetInitialTemporaryGlobalQuota,
                 weak_factory_.GetWeakPtr()));
}
