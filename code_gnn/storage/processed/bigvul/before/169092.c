void ReportPageHistogramsAfterAccess(const OfflinePageItem& offline_page_item,
                                     const base::Time& access_time) {
  base::HistogramBase* histogram = base::Histogram::FactoryGet(
      AddHistogramSuffix(offline_page_item.client_id,
                         offline_page_item.access_count == 0
                             ? "OfflinePages.FirstOpenSinceCreated"
                             : "OfflinePages.OpenSinceLastOpen"),
      1, kMaxOpenedPageHistogramBucket.InMinutes(), 50,
      base::HistogramBase::kUmaTargetedHistogramFlag);
  histogram->Add(
      (access_time - offline_page_item.last_access_time).InMinutes());
}
