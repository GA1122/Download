void AppCacheUpdateJob::Cancel() {
  internal_state_ = CANCELLED;

  LogHistogramStats(CANCELLED_ERROR, GURL());

  if (manifest_fetcher_) {
    delete manifest_fetcher_;
    manifest_fetcher_ = NULL;
  }

  for (PendingUrlFetches::iterator it = pending_url_fetches_.begin();
       it != pending_url_fetches_.end(); ++it) {
    delete it->second;
  }
  pending_url_fetches_.clear();

  for (PendingUrlFetches::iterator it = master_entry_fetches_.begin();
       it != master_entry_fetches_.end(); ++it) {
    delete it->second;
  }
  master_entry_fetches_.clear();

  ClearPendingMasterEntries();
  DiscardInprogressCache();

  if (manifest_response_writer_)
    manifest_response_writer_.reset();

  storage_->CancelDelegateCallbacks(this);
}
