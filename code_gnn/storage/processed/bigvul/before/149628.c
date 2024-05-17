void ResourcePrefetchPredictor::OnHistoryAndCacheLoaded() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK_EQ(INITIALIZING, initialization_state_);

  initialization_state_ = INITIALIZED;
  if (delete_all_data_requested_) {
    DeleteAllUrls();
    delete_all_data_requested_ = false;
  }
  if (observer_)
    observer_->OnPredictorInitialized();
}
