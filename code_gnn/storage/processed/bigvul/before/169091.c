void ReportPageHistogramAfterSave(
    ClientPolicyController* policy_controller_,
    const std::map<int64_t, OfflinePageItem>& offline_pages,
    const OfflinePageItem& offline_page,
    const base::Time& save_time) {
  DCHECK(policy_controller_);
  base::HistogramBase* histogram = base::Histogram::FactoryTimeGet(
      AddHistogramSuffix(offline_page.client_id, "OfflinePages.SavePageTime"),
      base::TimeDelta::FromMilliseconds(1), base::TimeDelta::FromSeconds(10),
      50, base::HistogramBase::kUmaTargetedHistogramFlag);
  histogram->AddTime(save_time - offline_page.creation_time);

  histogram = base::Histogram::FactoryGet(
      AddHistogramSuffix(offline_page.client_id, "OfflinePages.PageSize"), 1,
      10000, 50, base::HistogramBase::kUmaTargetedHistogramFlag);
  histogram->Add(offline_page.file_size / 1024);

  if (policy_controller_->IsSupportedByDownload(
          offline_page.client_id.name_space)) {
    int matching_url_count;
    base::TimeDelta time_since_most_recent_duplicate;
    if (GetMatchingURLCountAndMostRecentCreationTime(
            offline_pages, offline_page.client_id.name_space, offline_page.url,
            offline_page.creation_time, &matching_url_count,
            &time_since_most_recent_duplicate)) {
      UMA_HISTOGRAM_CUSTOM_COUNTS(
          "OfflinePages.DownloadSavedPageTimeSinceDuplicateSaved",
          time_since_most_recent_duplicate.InSeconds(),
          base::TimeDelta::FromSeconds(1).InSeconds(),
          base::TimeDelta::FromDays(7).InSeconds(), 50);
    }
    UMA_HISTOGRAM_CUSTOM_COUNTS("OfflinePages.DownloadSavedPageDuplicateCount",
                                matching_url_count, 1, 20, 10);
  }
}
