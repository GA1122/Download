void ReportPageHistogramsAfterDelete(
    const std::map<int64_t, OfflinePageItem>& offline_pages,
    const std::vector<OfflinePageItem>& deleted_pages,
    const base::Time& delete_time) {
  const int max_minutes = base::TimeDelta::FromDays(365).InMinutes();
  int64_t total_size = 0;

  for (const auto& page : deleted_pages) {
    total_size += page.file_size;
    ClientId client_id = page.client_id;

    if (client_id.name_space == kDownloadNamespace) {
      int remaining_pages_with_url;
      GetMatchingURLCountAndMostRecentCreationTime(
          offline_pages, page.client_id.name_space, page.url, base::Time::Max(),
          &remaining_pages_with_url, nullptr);
      UMA_HISTOGRAM_CUSTOM_COUNTS(
          "OfflinePages.DownloadDeletedPageDuplicateCount",
          remaining_pages_with_url, 1, 20, 10);
    }

    base::HistogramBase* histogram = base::Histogram::FactoryGet(
        AddHistogramSuffix(client_id, "OfflinePages.PageLifetime"), 1,
        max_minutes, 100, base::HistogramBase::kUmaTargetedHistogramFlag);
    histogram->Add((delete_time - page.creation_time).InMinutes());

    histogram = base::Histogram::FactoryGet(
        AddHistogramSuffix(client_id,
                           "OfflinePages.DeletePage.TimeSinceLastOpen"),
        1, max_minutes, 100, base::HistogramBase::kUmaTargetedHistogramFlag);
    histogram->Add((delete_time - page.last_access_time).InMinutes());

    histogram = base::Histogram::FactoryGet(
        AddHistogramSuffix(client_id,
                           "OfflinePages.DeletePage.LastOpenToCreated"),
        1, max_minutes, 100, base::HistogramBase::kUmaTargetedHistogramFlag);
    histogram->Add((page.last_access_time - page.creation_time).InMinutes());

    histogram = base::Histogram::FactoryGet(
        AddHistogramSuffix(client_id, "OfflinePages.DeletePage.PageSize"), 1,
        10000, 50, base::HistogramBase::kUmaTargetedHistogramFlag);
    histogram->Add(page.file_size / 1024);

    histogram = base::Histogram::FactoryGet(
        AddHistogramSuffix(client_id, "OfflinePages.DeletePage.AccessCount"), 1,
        1000000, 50, base::HistogramBase::kUmaTargetedHistogramFlag);
    histogram->Add(page.access_count);
  }

  if (deleted_pages.size() > 1) {
    UMA_HISTOGRAM_COUNTS("OfflinePages.BatchDelete.Count",
                         static_cast<int32_t>(deleted_pages.size()));
    UMA_HISTOGRAM_MEMORY_KB("OfflinePages.BatchDelete.TotalPageSize",
                            total_size / 1024);
  }
}
