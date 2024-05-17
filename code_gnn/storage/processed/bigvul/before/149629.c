void ResourcePrefetchPredictor::OnHistoryServiceLoaded(
    history::HistoryService* history_service) {
  if (initialization_state_ == INITIALIZING) {
    OnHistoryAndCacheLoaded();
  }
}
