void AppCacheUpdateJob::Cancel() {
  internal_state_ = CANCELLED;

  LogHistogramStats(CANCELLED_ERROR, GURL());

  if (manifest_fetcher_) {
    delete manifest_fetcher_;
    manifest_fetcher_ = nullptr;
  }

  for (auto& pair : pending_url_fetches_)
    delete pair.second;
  pending_url_fetches_.clear();

  for (auto& pair : master_entry_fetches_)
    delete pair.second;
  master_entry_fetches_.clear();

  ClearPendingMasterEntries();
  DiscardInprogressCache();

  if (manifest_response_writer_)
    manifest_response_writer_.reset();

  storage_->CancelDelegateCallbacks(this);
}
