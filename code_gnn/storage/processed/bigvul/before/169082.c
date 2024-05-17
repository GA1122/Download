void OfflinePageModelImpl::OnInitialGetOfflinePagesDone(
    const base::TimeTicks& start_time,
    std::vector<OfflinePageItem> offline_pages) {
  DCHECK(!is_loaded_);

  UMA_HISTOGRAM_TIMES("OfflinePages.Model.ConstructionToLoadedEventTime",
                      base::TimeTicks::Now() - start_time);

  CacheLoadedData(offline_pages);
  FinalizeModelLoad();

  CheckMetadataConsistency();
}
